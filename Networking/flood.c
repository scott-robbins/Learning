#include <stdio.h>	//for printf
#include <string.h> //memset
#include <sys/socket.h>	//for socket ofcourse
#include <stdlib.h> //for exit(0);
#include <errno.h> //For errno - the error number
#include <netinet/tcp.h>	//Provides declarations for tcp header
#include <netinet/ip.h>	//Provides declarations for ip header
#include <arpa/inet.h> // inet_addr
#include <unistd.h> // sleep()

void usage(){
	printf("Incorrect Usage\n");
	printf("$ tcp_packets <ip_address> <port>\n");
}


int main(int argc, char const *argv[])
{	
	// check args
	if(argc < 3){
		usage();
		exit(0);
	}
	// Determine port to connect send packets to 
	int port = atoi(argv[2]);
	printf("[*] Sending Traffic to %s:%d\n", argv[1], port);

	// setup the payload
	char buff[1024];
	strcpy(buff, "OH HEY FRIEND :D");

	int n = 0;
	while(1)
	{
		 // setup 
		int clientSocket;
		struct sockaddr_in serverAddr;
		socklen_t addrSize;			

		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(argv[2]);
		serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
		addrSize = sizeof serverAddr;
	  	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero); /* Set all bits of the padding field to 0 */

		//create socket 
		clientSocket = socket(PF_INET, SOCK_STREAM, 0);
		//connect
		connect(clientSocket, (struct sockaddr *) &serverAddr, addrSize);
		// send some data
		// send(clientSocket, buff, 1024, 0);
		if(n % 1000 == 0){
			printf("Connections Made: %d\n", n);
		}
		// sleep(0.1);
		n = n + 1;
	}


	return 0;
}