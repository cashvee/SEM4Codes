#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 1024
#define PORT 3388

int main() {
    int sockfd, retval;
    struct sockaddr_in serveraddr;
    char filename[MAXSIZE], buffer[MAXSIZE];
    int option;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        perror("Connection failed");
        return -1;
    }

    printf("Enter filename: ");
    scanf("%s", filename);
    send(sockfd, filename, strlen(filename), 0);

    recv(sockfd, buffer, sizeof(buffer), 0);
    if (strcmp(buffer, "File not present") == 0) {
        printf("%s\n", buffer);
        close(sockfd);
        return 0;
    }

    while (1) {
        printf("\n1. Search\n2. Replace\n3. Reorder\n4. Exit\nEnter option: ");
        scanf("%d", &option);
        send(sockfd, &option, sizeof(option), 0);

        if (option == 1) {
            printf("Enter string to search: ");
            scanf("%s", buffer);
            send(sockfd, buffer, strlen(buffer), 0);
            recv(sockfd, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
        } else if (option == 2) {
            char str1[MAXSIZE], str2[MAXSIZE];
            printf("Enter word to replace: ");
            scanf("%s", str1);
            printf("Enter new word: ");
            scanf("%s", str2);
            send(sockfd, str1, strlen(str1), 0);
            send(sockfd, str2, strlen(str2), 0);
            recv(sockfd, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
        } else if (option == 3) {
            recv(sockfd, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
        } else if (option == 4) {
            break;
        }
    }

    close(sockfd);
    return 0;
}
