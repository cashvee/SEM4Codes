#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void sort_numbers(char *str, char *result) {
    char nums[BUFFER_SIZE] = {0};
    int count = 0;

    // Extract numbers
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9')
            nums[count++] = str[i];
    }

    // Sort numbers in ascending order
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (nums[i] > nums[j]) {
                char temp = nums[i];
                nums[i] = nums[j];
                nums[j] = temp;
            }
        }
    }

    // Format result
    sprintf(result, "Child Process (PID: %d) - Sorted Numbers: %s", getpid(), nums);
}

void sort_characters(char *str, char *result) {
    char chars[BUFFER_SIZE] = {0};
    int count = 0;

    // Extract characters
    for (int i = 0; str[i] != '\0'; i++) {
        if ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z'))
            chars[count++] = str[i];
    }

    // Sort characters in descending order
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (chars[i] < chars[j]) {
                char temp = chars[i];
                chars[i] = chars[j];
                chars[j] = temp;
            }
        }
    }

    // Format result
    sprintf(result, "Parent Process (PID: %d) - Sorted Characters: %s", getpid(), chars);
}

int main() {
    int sockfd, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE], result[BUFFER_SIZE];

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

    // Bind socket
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    // Listen for connections
    if (listen(sockfd, 5) == -1) {
        perror("Listening failed");
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept client connection
    addr_size = sizeof(client_addr);
    new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_size);
    if (new_sockfd == -1) {
        perror("Connection acceptance failed");
        exit(1);
    }

    // Receive string from client
    recv(new_sockfd, buffer, BUFFER_SIZE, 0);
    buffer[strcspn(buffer, "\n")] = '\0';

    // Fork process
    pid_t pid = fork();

    if (pid == 0) {  // Child process
        sort_numbers(buffer, result);
        send(new_sockfd, result, strlen(result), 0);
        exit(0);
    } else {  // Parent process
        wait(NULL); // Wait for child process
        sort_characters(buffer, result);
        send(new_sockfd, result, strlen(result), 0);
    }

    // Close sockets
    close(new_sockfd);
    close(sockfd);
    return 0;
}
