#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 50

int main() {
    char buff[MAXSIZE];
    int sockfd, retval, i;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("\nSocket creation error");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Connection error");
        return 1;
    }

    for (i = 0; ; i++) {
        memset(buff, '\0', sizeof(buff));
        printf("Enter the text (or 'stop' to quit): ");
        scanf("%s", buff);

        int s = strlen(buff) * sizeof(char);
        sentbytes = send(sockfd, buff, s, 0);
        if (sentbytes == -1) {
            printf("Send error!");
            close(sockfd);
            return 1;
        }

        // Check if the client wants to stop
        if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'o' && buff[3] == 'p') {
            break;
        }

        memset(buff, '\0', sizeof(buff));
        recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1) {
            printf("Receive error!");
            close(sockfd);
            return 1;
        }

        // Display the response from the server (Palindrome check result)
        printf("%s\n", buff);

        // Check if the server said "stop" to terminate
        if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'o' && buff[3] == 'p') {
            break;
        }
    }

    close(sockfd);
    return 0;
}
