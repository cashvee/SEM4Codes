#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_CLIENTS 5

// Function to check if two strings are anagrams
int is_anagram(char *str1, char *str2) {
    int count[256] = {0}; 
    while (*str1) count[(int)(*str1++)]++;
    while (*str2) count[(int)(*str2--)]--;
    for (int i = 0; i < 256; i++)
        if (count[i] != 0) return 0;
    return 1;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    char str1[100], str2[100], buffer[256];

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, MAX_CLIENTS);
    printf("Server listening on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len);
        
        // Get current time
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        printf("Connected to client %s:%d at %02d:%02d:%02d\n",
               inet_ntoa(address.sin_addr), ntohs(address.sin_port),
               t->tm_hour, t->tm_min, t->tm_sec);

        // Receive two strings
        recv(new_socket, str1, sizeof(str1), 0);
        recv(new_socket, str2, sizeof(str2), 0);

        // Check if they are anagrams
        if (is_anagram(str1, str2))
            strcpy(buffer, "Strings are anagrams.");
        else
            strcpy(buffer, "Strings are NOT anagrams.");
        
        send(new_socket, buffer, strlen(buffer) + 1, 0);
        close(new_socket);
    }

    return 0;
}
