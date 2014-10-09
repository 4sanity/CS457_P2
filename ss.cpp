/*
 * Derek Drummond and Sam Bruns
 * PA2 - ss.cpp code file
 * Due: 09/12/14
*/

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BACKLOG 10

using namespace std;

//////////////////////////////////////////////////////////////////////////////FUNCTION PROTOTYPES//////////////////////////////////////////////////////////////////////////////////////////////////////
int startListening(int port);
int tearDown(int servSock_fd);
/////////////////////////////////////////////////////////////////////////////END FUNCTION PROTOTYPES///////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]){
	int port;
	int serv_sockfd, client_sockfd;//socket file descriptors that will be used for TCP connection
	socklen_t clientAddrLen = sizeof(struct sockaddr_storage);
	struct sockaddr_storage client;
	memset(&client, 0, sizeof(struct sockaddr_storage));
	
	//port argument is optional
	if(argc==1){
		port=0;
	}
	else if(argc == 2){
		for(int i = 0; i < strlen(argv[1]); ++i){
			if (!isdigit(argv[1][i])){
				cerr << "USAGE: An integer value must be provided for <port#>." << endl;
				cerr << "./ss 1337" << endl;
				break;
			}
		}
		port = atoi(argv[1]);
	}
	else{
		cerr << "USAGE: The correct # of arguments was not provided." << endl;
		cerr << "./ss <Port#>" << endl;
	}
	
	serv_sockfd = startListening(port);
	
	if((client_sockfd = accept(serv_sockfd, (struct sockaddr*)&client, &clientAddrLen)),0){
		cerr << "Connection could not be established" << endl;
		exit(EXIT_FAILURE);
	}
	
	while(true){
		char URL[1000];
		recv(client_sockfd,URL,sizeof(URL),0);
		cout << "Request: " << URL << endl;
		
		char filename[1000];  //probably wont use filename
		recv(client_sockfd,filename,sizeof(filename),0);
		
		//dont touch this part because i dont even know how it works
		int bytesRead = 1;
		int bytesSent;
		char buff[1000];
		int chains = open("chainfile.txt",O_CREAT | O_WRONLY,S_IRUSR | S_IWUSR);
		if(chains == -1){
			cout << "Error: Could not open file." << endl;
		}
		while(bytesRead > 0){      
			bytesRead = recv(client_sockfd, buff, 1000, 0);
			if(bytesRead == 0){
				break;
			}
			bytesSent = write(chains, buff, bytesRead);
			if(bytesSent < 0){
				cout << "Error: Message failed to send." << endl;
			}
		}
		
		//this probably doesnt need to be here, when this is threaded and awget is called it will print this out
		cout << "chainlist is " << endl;
		ifstream readIn("chainfile.txt", ifstream::in);
		int cnt;
		readIn >> cnt;
		string newline;
		getline(readIn,newline);
		string line;
		for (int i = 0; i<cnt; i++){
			getline(readIn,line);
			cout << "<" << line << ">" << endl;
		}
		//cout << "next SS is <" << dest.first << "," << dest.second << ">" << endl;
		cout << "waiting for file..." << endl;

		//this is where you thread and check if cnt is 0, it is popped in awget
		
		exit(1);
	}
	
}//End Main


int startListening(int serverPort){
	
	int serverSocket;
	struct sockaddr_in serverAddress;
	
	if((serverSocket=socket(PF_INET,SOCK_STREAM,0))<0){
		cout << "Error: Server socket could not be created." << endl;
		exit(1);
	}
		
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //LOOPBACK binds to localhost
	serverAddress.sin_port = htons(serverPort);
		
	if(bind(serverSocket,(struct sockaddr*)&serverAddress,sizeof(serverAddress))<0){
		cout << "Error: Server failed to bind." << endl;
		exit(1);
	}
		
	if(listen(serverSocket, BACKLOG)<0){
		cout << "Error: Server failed to start listen." << endl;
		exit(1);
	}
		
	//gethostname then store IP and port server is on then print
	string SADDRESS;
	char buff[80];
   	gethostname(buff, sizeof(buff));
    struct hostent *IP = gethostbyname(buff);
    for (int i = 0; IP->h_addr_list[i] != 0; ++i) {
      	struct in_addr addr;
      	memcpy(&addr, IP->h_addr_list[i], sizeof(struct in_addr));
		SADDRESS = inet_ntoa(addr);
    }
	socklen_t length = sizeof(serverAddress);
   	getsockname(serverSocket,(struct sockaddr*)&serverAddress,&length);
	cout << "Waiting for a connection on " << SADDRESS << " port " << ntohs(serverAddress.sin_port) << endl;
	
	return serverSocket;

}
//End startListening

/* This function is responsible for keeping the anonymity of the get. In order to do this the local copy of the file retrieved from wget needs to be deleted and the TCP connection needs to be closed.
 * The file can be deleted using the system() call. We want to run "rm -f " + URL which will remove the file from the current directory.
 *
 *	PARAMETERS
 *		servSock_fd - The socket file descriptor for this connection.
 *
 *	RETURN
 *		int - 0 for succesful tear down, -1 if errors occured.
 */
int tearDown(int servSock_fd){
}
//End tearDownConnection