#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    int choice;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Get user input
    printf("Enter option:\n1. Registration Number\n2. Name\n3. Subject Code\nChoice: ");
    scanf("%d", &choice);
    getchar(); // Consume newline

    printf("Enter details: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline

    // Send option and input to server
    send(sock, &choice, sizeof(choice), 0);
    send(sock, buffer, strlen(buffer), 0);

    // Receive response
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server Response: %s\n", buffer);

    // Close socket
    close(sock);
    return 0;
}
