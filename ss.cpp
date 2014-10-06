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


#define BACKLOG 10

using namespace std;

//////////////////////////////////////////////////////////////////////////////FUNCTION PROTOTYPES//////////////////////////////////////////////////////////////////////////////////////////////////////
int startListening(int servPort);
int tearDown(int servSock_fd);
/////////////////////////////////////////////////////////////////////////////END FUNCTION PROTOTYPES///////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]){

	int servPort; //Port Number this host will listen on.
	int serv_sockfd;//socket file descriptor that will be used for TCP connection
	
	if(argc == 2){
		for(int i = 0; i < strlen(argv[1]); ++i){
			if (!isdigit(argv[1][i])){
				cerr << "USAGE: An integer value must be provided for <port#>." << endl;
				cerr << "./ss 1337" << endl;
				break;
			}
		}
		servPort = atoi(argv[1]);
	}
	else{
		cerr << "USAGE: The correct # of arguments was not provided." << endl;
		cerr << "./ss <Port#>" << endl;
	}
	
	startListening(servPort);
	//Print our IP and Port number being listened on here.
	
}//End Main

/*
 * Essentially this is the server code from the chat program written earlier.
 * The function stops after the initial listen call is made, therefore the accept() call needs to be made in main.
 *
 *	PARAMETERS
 *		servPort - The port this server will listen on.
 *
 *	RETURN
 *		int - The socket file descriptor for this server.
 */
int startListening(int serverPort){

	int sock_fd;
	struct addrinfo hints, *retVal, *curr;//Used for getaddrinfo()
	struct sockaddr_in server_sin, client_sin;
	struct sockaddr_storage client_addr;
		
	//Initialize and set values for hints
	memset(&hints, 0, sizeof(struct addrinfo))	;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;//Signals any protocol
	
	if(getaddrinfo("www.colostate.edu", NULL, &hints, &retVal) != 0){
		cerr << "getaddrinfo() Failed" << endl;
		exit(EXIT_FAILURE);
	}
	
	for(curr = retVal; curr != NULL; curr = curr->ai_next)	{
		sock_fd = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol);
		if(sock_fd == -1){
			continue;
		}
		if(bind(sock_fd, curr->ai_addr, curr->ai_addrlen)){
			break;
		}
		close(sock_fd);
	}
	
	if(curr == NULL){
		cerr << "Socket could not bind a socket" << endl;
		exit(EXIT_FAILURE);
	}
	
	//Need to print out the IP and port for this server here
	
	return listen(sock_fd, BACKLOG);//BACKLOG is defined as 10.
	

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

























