#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    pid_t pid;

    // Creating socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Setting address properties
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept client connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Forking process
    pid = fork();
    if (pid == 0) { // Child process (handles receiving messages)
        printf("Child process: PID = %d, PPID = %d\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            read(new_socket, buffer, BUFFER_SIZE);
            printf("Client: %s\n", buffer);
        }
    } else { // Parent process (handles sending messages)
        printf("Parent process: PID = %d, PPID = %d\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            printf("Server: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            send(new_socket, buffer, strlen(buffer), 0);
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
