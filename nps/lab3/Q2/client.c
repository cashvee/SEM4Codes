#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_SIZE 100

int main() {
    int sockfd;
    char buffer[MAX_SIZE], recv_buffer[MAX_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_size = sizeof(server_addr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Get input string from user
    printf("Enter a string: ");
    scanf("%s", buffer);

    // Send string to server
    sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&server_addr, addr_size);
    printf("String sent to server.\n");

    // Receive and print permutations
    printf("Received permutations:\n");
    while (recvfrom(sockfd, recv_buffer, MAX_SIZE, 0, (struct sockaddr *)&server_addr, &addr_size) > 0) {
        printf("%s\n", recv_buffer);
    }

    // Close the socket
    close(sockfd);
    return 0;
}
