#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[])
{

  if(argc < 3){
    printf("Incorrect Usage\n");
    printf("$ client <remote_ip>\n");
  }

  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(atoi(argv[2]));
  /* Set IP address for server socket connection endpoint */
  serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
  /*---- Read the message from the server into the buffer ----*/
  recv(clientSocket, buffer, 1024, 0);
  /*---- Print the received message ----*/
  printf("Data received: %s",buffer);   

  return 0;
}