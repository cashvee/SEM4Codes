#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char input[BUFFER_SIZE], result[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(1);
    }

    // Get input from user
    printf("Enter an alphanumeric string: ");
    scanf("%s", input);

    // Send input string to server
    send(sockfd, input, strlen(input), 0);

    // Receive sorted numbers from child process
    recv(sockfd, result, BUFFER_SIZE, 0);
    printf("%s\n", result);

    // Receive sorted characters from parent process
    recv(sockfd, result, BUFFER_SIZE, 0);
    printf("%s\n", result);

    // Close socket
    close(sockfd);
    return 0;
}
