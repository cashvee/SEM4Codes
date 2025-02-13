#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char str1[100], str2[100], buffer[256];

    // Create client socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Get input from user
    printf("Enter first string: ");
    scanf("%s", str1);
    printf("Enter second string: ");
    scanf("%s", str2);

    // Send strings to server
    send(sock, str1, strlen(str1) + 1, 0);
    send(sock, str2, strlen(str2) + 1, 0);

    // Receive result
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);

    // Close connection
    close(sock);
    return 0;
}
