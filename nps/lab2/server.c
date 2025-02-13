#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAXSIZE 1024
#define PORT 3388

void searchString(int client_sock, char *filename);
void replaceString(int client_sock, char *filename);
void reorderFile(int client_sock, char *filename);

int main() {
    int server_sock, client_sock, retval;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t addr_size;
    char filename[MAXSIZE], buffer[MAXSIZE];

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        return -1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(server_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        perror("Bind failed");
        close(server_sock);
        return -1;
    }

    listen(server_sock, 1);
    addr_size = sizeof(clientaddr);
    client_sock = accept(server_sock, (struct sockaddr *)&clientaddr, &addr_size);
    if (client_sock == -1) {
        perror("Accept failed");
        close(server_sock);
        return -1;
    }

    // Receive filename from client
    recv(client_sock, filename, sizeof(filename), 0);

    FILE *file = fopen(filename, "r");
    if (!file) {
        strcpy(buffer, "File not present");
        send(client_sock, buffer, strlen(buffer), 0);
        close(client_sock);
        close(server_sock);
        return 0;
    }
    fclose(file);

    while (1) {
        int option;
        recv(client_sock, &option, sizeof(option), 0);

        if (option == 1) {
            searchString(client_sock, filename);
        } else if (option == 2) {
            replaceString(client_sock, filename);
        } else if (option == 3) {
            reorderFile(client_sock, filename);
        } else if (option == 4) {
            break;
        }
    }

    close(client_sock);
    close(server_sock);
    return 0;
}

void searchString(int client_sock, char *filename) {
    char search[MAXSIZE], buffer[MAXSIZE];
    recv(client_sock, search, sizeof(search), 0);

    FILE *file = fopen(filename, "r");
    int count = 0;
    while (fscanf(file, "%s", buffer) != EOF) {
        if (strcmp(buffer, search) == 0) count++;
    }
    fclose(file);

    if (count > 0) {
        sprintf(buffer, "String found %d times", count);
    } else {
        strcpy(buffer, "String not found");
    }
    send(client_sock, buffer, strlen(buffer), 0);
}

void replaceString(int client_sock, char *filename) {
    char str1[MAXSIZE], str2[MAXSIZE], buffer[MAXSIZE];
    recv(client_sock, str1, sizeof(str1), 0);
    recv(client_sock, str2, sizeof(str2), 0);

    FILE *file = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");
    int found = 0;

    while (fscanf(file, "%s", buffer) != EOF) {
        if (strcmp(buffer, str1) == 0) {
            fprintf(temp, "%s ", str2);
            found = 1;
        } else {
            fprintf(temp, "%s ", buffer);
        }
    }
    fclose(file);
    fclose(temp);

    remove(filename);
    rename("temp.txt", filename);

    strcpy(buffer, found ? "String replaced" : "String not found");
    send(client_sock, buffer, strlen(buffer), 0);
}

void reorderFile(int client_sock, char *filename) {
    char words[MAXSIZE][MAXSIZE], buffer[MAXSIZE];
    int count = 0;

    FILE *file = fopen(filename, "r");
    while (fscanf(file, "%s", words[count]) != EOF) count++;
    fclose(file);

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char temp[MAXSIZE];
                strcpy(temp, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], temp);
            }
        }
    }

    file = fopen(filename, "w");
    for (int i = 0; i < count; i++) fprintf(file, "%s ", words[i]);
    fclose(file);

    strcpy(buffer, "File content reordered");
    send(client_sock, buffer, strlen(buffer), 0);
}
