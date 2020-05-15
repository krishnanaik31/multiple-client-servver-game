#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
int main(int argc, char *argv[]){
char message[100];
long int ss=0;
int dice = 0;
int converted_number, number_to_send;
int server, portNumber;
socklen_t len;
struct sockaddr_in servAdd;
	if(argc != 3){
		printf("Call model:%s <IP> <Port#>\n",argv[0]);
		exit(0);
	}
	if ((server=socket(AF_INET,SOCK_STREAM,0))<0){
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}
	if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
		fprintf(stderr, "connect() failed, exiting\n");
		exit(3);
	}
	while(1) {
		if (read(server, message, 100)<0){
			fprintf(stderr, "read() error\n");
			exit(3);
		}
		if (strcmp(message, "You can play now")==0){
			fprintf(stderr, "%s\n", message);
			dice =(int) time(&ss)%10 + 1;
			printf("Obtained Score: %d\n", dice);
			converted_number = htonl(dice);
			write(server, &converted_number, sizeof(converted_number));
		}
		if (strcmp(message, "Game over: you won the game")==0){
 			fprintf(stderr, "%s\n", "I won the game");
			break;
		}
		else if (strcmp(message,"Game over: you lost the game")==0){
			fprintf(stderr, "%s\n", "I lost the game");
			break;
		}
	}
	exit(0);
}

