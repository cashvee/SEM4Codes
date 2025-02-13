#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_SIZE 100

// Function to swap characters
void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

// Function to generate permutations recursively
void permute(char *str, int l, int r, int sockfd, struct sockaddr_in *client_addr, socklen_t addr_size) {
    if (l == r) {
        sendto(sockfd, str, strlen(str) + 1, 0, (struct sockaddr *)client_addr, addr_size);
        usleep(10000);  // Small delay to avoid packet loss
    } else {
        for (int i = l; i <= r; i++) {
            swap(&str[l], &str[i]);
            permute(str, l + 1, r, sockfd, client_addr, addr_size);
            swap(&str[l], &str[i]);  // Backtrack
        }
    }
}

int main() {
    int sockfd;
    char buffer[MAX_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Receive string from client
    recvfrom(sockfd, buffer, MAX_SIZE, 0, (struct sockaddr *)&client_addr, &addr_size);
    printf("Received string: %s\n", buffer);

    // Generate and send permutations
    int len = strlen(buffer);
    printf("Permutations being sent:\n");
    permute(buffer, 0, len - 1, sockfd, &client_addr, addr_size);

    // Close the socket
    close(sockfd);
    return 0;
}
