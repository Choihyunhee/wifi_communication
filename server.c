#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <wiringARTIK.h>

#define MYPORT 3490	
#define BACKLOG 10	
#define MAXDATASIZE 100
int main()
{
	int recv_data;
	int sockfd, new_fd; 
	int numbytes;
	struct sockaddr_in my_addr; 	
	struct sockaddr_in their_addr; 	
	int sin_size;
	int on = 1; 
	char buf[MAXDATASIZE];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;		
	my_addr.sin_port = htons(MYPORT);	
	my_addr.sin_addr.s_addr = INADDR_ANY;	
	bzero(&(my_addr.sin_zero), 8);		
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)); 
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) 
	{
		perror("bind");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	while(1) { 
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) 
		{
			perror("accept");
			continue;
		}
		if (!fork()) {	
			while(1){
				if ((numbytes=recv(new_fd, buf, MAXDATASIZE, 0)) == -1) {
					perror("recv");
					exit(1);
				}
				if(numbytes==0) continue;
				buf[numbytes] = '\0';

				recv_data = atoi(buf);
				if(recv_data == 999){ 
					printf("exit\n");
					return -1;
				}
				printf(" recv = %d \n", recv_data);		
			}
			close(new_fd); 			
			while(waitpid(-1,NULL,WNOHANG) > 0); 
		}
	}
}

