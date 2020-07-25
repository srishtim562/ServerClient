#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<pthread.h>
#include<string.h>
#include <netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>


void *runner(void *);

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Usage:./client IP address number of requests to be made\n");
		exit(0);
	}
	pthread_t tid[2];
	int i;
	int n = atoi(argv[2]);
	for(i = 0; i < n; i++)
	{
	pthread_create(&tid[i], NULL, runner,argv[1]);
	pthread_join(tid[i], NULL);
	}


	return 0;

}

void *runner(void *param)
{
	// CREATE A SOCKET
        char * n = (char*)(param);
	char fname[100];
	//char msg[]="Could not open file";
        int  network_socket = socket(AF_INET, SOCK_STREAM, 0);
        struct hostent *ptrh;
	
        //Specify an address for the socket
        struct sockaddr_in server_address;
	memset((char*)&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(9002);
        ptrh = gethostbyname(n);
	memcpy(&server_address.sin_addr,ptrh->h_addr,ptrh->h_length);

        while(connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address)));
	printf("\nEnter filename\n");
	scanf("%s",fname);
	send(network_socket,fname,sizeof(fname),0);

        //Receive data from the server*/
        char server_response[11];
	while(recv(network_socket, &server_response,sizeof(server_response),0)>0)
        {
                if(strcmp(server_response,"\a")==0)
		{
			printf("Could not open file\n");
			break;
		}
                printf("%s",server_response);
               
        }

	printf("\n");
	
        close(network_socket);
	pthread_exit(0);

}
