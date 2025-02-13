#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

void lookup_dns(char *domain, char *result) {
    FILE *file = fopen("database.txt", "r");
    if (!file) {
        perror("Database file missing");
        strcpy(result, "Error: Database not found");
        return;
    }

    char line[MAXLINE], db_domain[MAXLINE], ip[MAXLINE];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%s %s", db_domain, ip);
        if (strcmp(domain, db_domain) == 0) {
            strcpy(result, ip);
            fclose(file);
            return;
        }
    }

    strcpy(result, "Error: Domain not found");
    fclose(file);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[MAXLINE], response[MAXLINE];

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("DNS Server listening on port %d...\n", PORT);

    while (1) {
        // Accept client connection
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        // Read domain name from client
        recv(client_sock, buffer, sizeof(buffer), 0);

        // Look up domain in database
        lookup_dns(buffer, response);

        // Send response to client
        send(client_sock, response, strlen(response), 0);

        // Close client connection
        close(client_sock);
    }

    close(server_sock);
    return 0;
}
