
#include <string>
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <iostream>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
using namespace std;

class SimpleServer {       // The class
    public:             // Access specifier
        int servePort;        // Attribute (int variable)
        string name;  // Attribute (string variable)

    SimpleServer(){
        name = "SimpleServer";
        servePort = 4242;
    }

    int startServer(){
        char msgbuf[1500];  // message buffer
        // setup the socket 
        sockaddr_in srv;
        memset((char*)&srv, 0, sizeof(srv));
        srv.sin_family = AF_INET;
        srv.sin_addr.s_addr = htonl(INADDR_ANY); // bind to 0.0.0.0
        srv.sin_port = htons(servePort);

        // open socket 
        int server = socket(AF_INET, SOCK_STREAM, 0);
        if(server < 0){
            cerr << "[!!] Unable to create socket" << endl;
            exit(0);
        }

        // bind the socket
        int bindStatus = bind(server, (struct sockaddr*) &srv, sizeof(srv));
        if(bindStatus < 0){
            cerr << "[!!] Error Binding Socket to 0.0.0.0:" << servePort << endl;
            exit(0);
        }
        
        // Wait for clients to connect
        cout << "[*] Server listening on 0.0.0.0:" << servePort << "\n";
        listen(server, 10); // listen (up to 10 at a time)
        
        // acept a client
        sockaddr_in clientSock;
        socklen_t clientSockSize = sizeof(clientSock);
        
        // accept a client and handle the connection
        int csd = accept(server, (sockaddr *)&clientSock, &clientSockSize);
        if(csd < 0){
            cerr << "[!!] Error Accepting Connection from client" << endl;
            exit(1);
        }
        cout << "[*] Connection Accepted from " << inet_ntoa(clientSock.sin_addr) << endl;

        // Keep track of session time
        struct timeval start1, end1;
        gettimeofday(&start1, NULL);
        int bytesRcvd, bytesSent = 0;

        while(1){
            // recieve message from client
            memset(&msgbuf, 0, sizeof(msgbuf)); //clear msg buffer
            bytesRcvd += recv(csd, (char*)&msgbuf, sizeof(msgbuf), 0);
            if(strcmp(msgbuf, "#!#!")){
                cout << "[*] " << inet_ntoa(clientSock.sin_addr) << " has disconnected\n";
                break;
            }
        }
        // close sockets and stop the clock
        gettimeofday(&end1, NULL);
        close(csd);
        close(server);
        cout << "********Session********" << endl;
        cout << "Bytes written: " << bytesSent << " Bytes read: " << bytesRcvd << endl;
        cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << endl;
        cout << "Connection closed..." << endl;
        return 0;
    }
};


int main() {
    SimpleServer server;  // Create an object of MyClass
    cout << "[*] SimpleServer Created\n"; 
     // Start the server
    server.startServer();
    return 0;
}
