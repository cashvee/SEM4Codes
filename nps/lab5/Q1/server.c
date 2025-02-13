#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_CLIENTS 2

int client_count = 0;

void handle_client(int client_sock, struct sockaddr_in client_addr) {
    char buffer[1024] = {0};
    char final_message[1024] = "Manipal ";

    // Read data from client
    read(client_sock, buffer, sizeof(buffer));
    printf("Received from Client (Socket: %d): %s\n", ntohs(client_addr.sin_port), buffer);

    // Append received message
    strcat(final_message, buffer);

    // Send the final message when both clients have sent data
    client_count++;
    if (client_count == MAX_CLIENTS) {
        strcat(final_message, "\n");
        printf("Final Message: %s\n", final_message);
    }

    // If too many clients, terminate session
    if (client_count > MAX_CLIENTS) {
        char terminate_msg[] = "terminate session";
        send(client_sock, terminate_msg, sizeof(terminate_msg), 0);
        printf("Too many clients! Terminating all sessions.\n");
        close(client_sock);
        exit(0);
    }

    // Send response back to client
    send(client_sock, final_message, sizeof(final_message), 0);
    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address settings
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept new client connection
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Create child process for each client
        if (fork() == 0) {
            close(server_sock);
            handle_client(client_sock, client_addr);
            exit(0);
        }
        close(client_sock);
    }

    return 0;
}
