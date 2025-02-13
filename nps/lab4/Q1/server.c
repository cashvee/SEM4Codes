#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define PORT 8080

void handle_request(int client_sock) {
    int choice;
    char buffer[1024];

    // Receive choice
    recv(client_sock, &choice, sizeof(choice), 0);
    recv(client_sock, buffer, sizeof(buffer), 0);

    // Fork a child process
    pid_t pid = fork();

    if (pid == 0) { // Child process
        char response[1024];
        pid_t my_pid = getpid();

        if (choice == 1) {
            snprintf(response, sizeof(response), "PID %d: Name: John Doe, Address: XYZ Street", my_pid);
        } else if (choice == 2) {
            snprintf(response, sizeof(response), "PID %d: Dept: CSE, Semester: 5, Section: A", my_pid);
        } else if (choice == 3) {
            snprintf(response, sizeof(response), "PID %d: Marks in %s: 85", my_pid, buffer);
        } else {
            snprintf(response, sizeof(response), "Invalid choice");
        }

        send(client_sock, response, strlen(response), 0);
        close(client_sock);
        exit(0);
    } else { // Parent process
        wait(NULL); // Wait for child to complete
    }
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

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

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept client connection
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        handle_request(client_sock);
        close(client_sock);
    }

    close(server_sock);
    return 0;
}
