#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 256 // Increased the buffer size to 256 for the response

// Function to check if a string is a palindrome
int is_palindrome(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0; // Not a palindrome
        }
    }
    return 1; // It's a palindrome
}

// Function to count vowels and return the count
int count_vowels(char *str) {
    int count = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == 'a' || str[i] == 'e' || str[i] == 'i' || str[i] == 'o' || str[i] == 'u' ||
            str[i] == 'A' || str[i] == 'E' || str[i] == 'I' || str[i] == 'O' || str[i] == 'U') {
            count++;
        }
    }
    return count;
}

int main() {
    int sockfd, newsockfd, retval, i;
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE];
    char original_buff[MAXSIZE]; // To store the original input message

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation error");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    retval = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        perror("Binding error");
        close(sockfd);
        return 1;
    }

    retval = listen(sockfd, 1);
    if (retval == -1) {
        perror("Listening error");
        close(sockfd);
        return 1;
    }

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &actuallen);
    if (newsockfd == -1) {
        perror("Accept error");
        close(sockfd);
        return 1;
    }

    for (i = 0; ; i++) {
        memset(buff, '\0', sizeof(buff));
        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1) {
            perror("Receive error");
            close(sockfd);
            close(newsockfd);
            return 1;
        }

        if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'o' && buff[3] == 'p') {
            break;
        }

        // Save the original input for length and vowel count
        strcpy(original_buff, buff);

        // Count vowels in the original input
        int vowel_count = count_vowels(original_buff);
        int length = strlen(original_buff);

        // Check if the received message is a palindrome
        int is_pal = is_palindrome(original_buff);
        if (is_pal) {
            strcpy(buff, "Palindrome detected!");
        } else {
            strcpy(buff, "Palindrome not detected!");
        }

        // Prepare the response including length, vowels, and palindrome info
        char response[MAXSIZE];
        snprintf(response, sizeof(response), "Original text length: %d, Vowels: %d, %s", length, vowel_count, buff);

        // Send the response back to the client
        sentbytes = send(newsockfd, response, strlen(response), 0);
        if (sentbytes == -1) {
            perror("Send error");
            close(sockfd);
            close(newsockfd);
            return 1;
        }

        // Check again for "stop" to end the connection
        if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'o' && buff[3] == 'p') {
            break;
        }
    }

    close(sockfd);
    close(newsockfd);
    return 0;
}

