#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>

char server_message[256] = "You have reached the server";
void *clientHandler(void *param);



int main()
{

	pthread_t tid;
	int n = 2,len;
	//Create the server the socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	//Specify an address for the socket
        struct sockaddr_in server_address,client_address;
	
	memset((char*)&server_address,0,sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(9002);
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//Bind the socket to our specified IP and port
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	
	listen(server_socket, 5);
	while(1)
	{
	 int re;
	 int client_socket;
	 len =  sizeof(struct sockaddr_in);
	 client_socket = accept(server_socket, (struct sockaddr*)&client_address, &len);
	 
	 if(client_socket < 0)
	 {
            perror("ERROR in accepting\n");
            return 0;
	 }
	 re = pthread_create(&tid, NULL, clientHandler, &client_socket);
	 if(re)
	 {
            printf("ERROR return code from the pthread_create() is %d\n", re);
	 }

	}

	return 0;
}
void *clientHandler(void *param)
{	
	int  client_socket = *((int*)param);
	char fname[100];
	char errmsg[]="\a"; 
	recv(client_socket,fname,100,0);
	int f = open(fname,O_RDONLY);
	if (f==(signed int)(-1))
	{
		printf("Could not open file\n");
		close(f);
		send(client_socket,errmsg, sizeof(errmsg), 0);

	}
	else
	{
	char buff[11];
		while((read(f,buff,1))>0)
		{
		printf("%s",buff);
		send(client_socket,buff, sizeof(buff), 0);
		}
		close(f);
	}

	close(client_socket);
	pthread_exit(0);
}
