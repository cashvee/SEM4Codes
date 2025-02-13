#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#define MAXSIZE 150
int main()
{

	char buff[MAXSIZE];
	int sockfd,retval,i;
	int sentbytes;
	int recedbytes1;
	struct sockaddr_in serveraddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
		return;
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Connection error");
		return;
	}

	for (i = 0; ; i+=1)
	{
		memset(buff, '\0', sizeof(buff));
		printf("Enter string:");
		scanf("%s",buff);
		buff[strlen(buff)] = '\0';
		int s = strlen(buff) * sizeof(char);
		sentbytes=send(sockfd,buff,s,0);
		if(sentbytes==-1)
		{
			printf("!!");
			close(sockfd);
		}
		if (buff[0] == 's' && buff[1] == 't' && buff[2] == 'o' && buff[3] == 'p' && buff[4] == '\0')
		{
			break;
		}
		memset(buff, '\0', sizeof(buff));
		recedbytes1=recv(sockfd,buff,sizeof(buff),0);
		printf ("%s \n", buff);
		recedbytes1=recv(sockfd,buff,sizeof(buff),0);
		printf ("%s \n", buff);
		recedbytes1=recv(sockfd,buff,sizeof(buff),0);
		printf ("%s \n", buff);
	}
	close(sockfd);
	return 0;
}
