#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <wiringARTIK.h>

#define PORT 3490	
#define MAXDATASIZE 100 
//데이터를 보내는 역할
int main(int argc, char *argv[]) //두개의 인자 parameter가 뒤에 몇개 붙었는지 ->argc ->  ./client ->argv[0], ip주소->argv[1]

	float send_data;

	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; 

	if (argc != 2) { //./client와 iP Address
		fprintf(stderr, "usage: ./client <host-name>\n");
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) { //hostaddress로 변환
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { //소켓 생성 스트림 방식
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;	
	their_addr.sin_port = htons(PORT);	//포트넘버 세팅
	their_addr.sin_addr = *((struct in_addr *)he->h_addr); //IP address를 가져와 서버 Address로 적용
	bzero(&(their_addr.sin_zero), 8);	

	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) //커넥트
	{
		perror("connect");
		exit(1);
	}

	while(strcmp(buf,"quit")){
		printf(" send : ");
		scanf("%d",&send_data);
		sprintf(buf, "%d", send_data);

		if (send(sockfd, buf, strlen(buf), 0) == -1) {
			perror("send");
			exit(0);
		}
	
	}
	close(sockfd);

	return 0;
}
