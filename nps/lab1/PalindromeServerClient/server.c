#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

#define MAXSIZE 150

int main() {
    int sockfd, newsockfd, retval, i;
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation error");
        return -1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        perror("Binding error");
        close(sockfd);
        return -1;
    }

    retval = listen(sockfd, 1);
    if (retval == -1) {
        perror("Listen error");
        close(sockfd);
        return -1;
    }
    
    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &actuallen);
    if (newsockfd == -1) {
        perror("Accept error");
        close(sockfd);
        return -1;
    }

    for (i = 0; ; i++) {
        memset(buff, '\0', sizeof(buff));
        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1) {
            perror("Error receiving data");
            close(sockfd);
            close(newsockfd);
            return -1;
        } else if (recedbytes == 0) {
            printf("Connection closed by client\n");
            break;
        }

        if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'o' && buff[3] == 'p') {
            break;
        }

        int len = strlen(buff);
        int a = 0, e = 0, I = 0, o = 0, u = 0;
        int palindrome = 1;

        for (int j = 0; j < len / 2; j++) {
            switch(buff[j]) {
                case 'a': case 'A': a++; break;
                case 'e': case 'E': e++; break;
                case 'i': case 'I': I++; break;
                case 'o': case 'O': o++; break;
                case 'u': case 'U': u++; break;
            }
            switch(buff[len - j - 1]) {
                case 'a': case 'A': a++; break;
                case 'e': case 'E': e++; break;
                case 'i': case 'I': I++; break;
                case 'o': case 'O': o++; break;
                case 'u': case 'U': u++; break;
            }
            if (buff[j] != buff[len - j - 1]) {
                palindrome = 0;
            }
        }
	if(len % 2 == 1)
	{
		switch(buff[len / 2])
		{
		        case 'a': case 'A': a++; break;
		        case 'e': case 'E': e++; break;
		        case 'i': case 'I': I++; break;
		        case 'o': case 'O': o++; break;
		        case 'u': case 'U': u++; break;
		}
	}
        char messageBack1[MAXSIZE];
        if (palindrome) {
            strcpy(messageBack1, "It is palindrome!");
        } else {
            strcpy(messageBack1, "Not a palindrome");
        }
        sentbytes = send(newsockfd, messageBack1, strlen(messageBack1), 0);
        if (sentbytes == -1) {
            perror("Error sending palindrome result");
            close(sockfd);
            close(newsockfd);
            return -1;
        }
        char messageBack2[MAXSIZE];
        sprintf(messageBack2, "Length of string: %d\0", len);        
        sentbytes = send(newsockfd, messageBack2, strlen(messageBack2), 0);
        if (sentbytes == -1) {
            perror("Error sending length");
            close(sockfd);
            close(newsockfd);
            return -1;
        }
        char messageBack3[MAXSIZE];
        sprintf(messageBack3, "A: %d\nE: %d\nI: %d\nO: %d\nU: %d\0", a, e, I, o, u);
        sentbytes = send(newsockfd, messageBack3, strlen(messageBack3), 0);
        if (sentbytes == -1) {
            close(sockfd);
            close(newsockfd);
            return -1;
        }
    }

    close(sockfd);
    close(newsockfd);
    return 0;
}

