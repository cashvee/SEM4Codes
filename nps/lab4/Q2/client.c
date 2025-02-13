#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char domain[1024], response[1024];

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

    // Get domain from user
    printf("Enter domain name: ");
    fgets(domain, sizeof(domain), stdin);
    domain[strcspn(domain, "\n")] = 0; // Remove newline

    // Send domain to server
    send(sock, domain, strlen(domain), 0);

    // Receive response
    recv(sock, response, sizeof(response), 0);
    printf("Server Response: %s\n", response);

    // Close socket
    close(sock);
    return 0;
}
