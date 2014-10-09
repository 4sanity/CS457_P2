/*
 * Derek Drummond and Sam Bruns
 * PA2 - awget code file
 * Due: 09/12/14
*/

#include "awget.h"

using namespace std;

char URL[] = "www.google.com/index.html";//Default URL as specified by write-up
char filename[] = "chaingang.txt";//Default filename as specified by write-up

int main(int argc, char* argv[]){

	pair<string,string> dest;

	if(!parseArgs(argc, argv)){
		cerr << "Usage: URL to get or filename of stepping stones to use was not specified.\n awget www.google.com -c stones.txt " << endl;
		exit(EXIT_FAILURE);
	}
	
	dest = selectRandomStep();
	
	//This whole If block is probably unnecessary but it catches the case where the chaingang.txt file has no entries...in no way is this anonymous but you still get the file...booyah!
	/*if(strncmp(get<0>(dest).c_str(), "127.0.0.1", get<0>(dest).length()-1) == 0 && strncmp(get<1>(dest).c_str(), "0", 1) == 0){
		cout << "No stepping stones were provided in the file " << filename << " fetching the content at " << URL << "." << endl;
		//system(wget(URL));
		exit(EXIT_SUCCESS);
	} */
	if(dest==NULL){
		cout << "No stepping stones were provided in the file " << filename << " fetching the content at " << URL << "." << endl;
	}
	
	int socket_fd = setupConnection(dest);
	if(socket_fd == -1){
		cerr << "A server could not be contacted. This most likely means the FBI have seized those systems and are tracking your IP at this very instant. Fly, you fool!" << endl;
		exit(EXIT_FAILURE);
	}
	
	/*After setupConnection has completed we have the socket descriptor associated with a specific port and are ready to communicate with the first server.
	 * There are 2 big questions we need to answer before implementing all of this though.
	 *      1.) How are we sending the list of stepping stones across the connection? 
	 *		A: So far it looks like a stringstream is the best way of transmitting a file. I used the SO topic http://stackoverflow.com/questions/7122254/bad-file-descriptor-with-bsd-socket
	 *			Essentially what this means is we will send each line of the file one packet at a time and reconstruct it into a new file on the server side.
	 *			We don't even have to keep a local copy this way. All that would occur is a stepping stones file would exist in memory and be lost when the program exits.
	 *	2.) Are we going to branch new connections for multiple requests or use the select command?
	 *		A: With the SO topic referenced above it looks like select is nice and simple.
	*/
	
}//End Main


int parseArgs(int argc, char* argv[]){
	if(argc==1){
		//no arguments, defaults are set
		return true;
	}else if(argc==2){
		//only argument given is URL, filename default is already set
		strcpy(URL,argv[1]);
		return true;
	}else if(argc==3){
		//option -c is given with filename and no URL
		string option = argv[1];
		if(!(option=="-c")){ return false; }
		strcpy(filename,argv[2]);
		return true;
	}else if(argc==4){
		//option -c is given with filename
		strcpy(URL,argv[1]);
		string option = argv[2];
		if(!(option=="-c")){ return false; }
		strcpy(filename,argv[3]);
		return true;
	}else{ 
		return false;
	}	
}
//End parseArgs

pair<string,string> selectRandomStep(){
	
	int cnt = 0;
	int randomIndex;
	pair<string,string> destinationHost = make_pair("127.0.0.1", "0");
	ifstream readIn(filename, ifstream::in);
	
	if(!readIn){
		cerr << "The file \"" << filename << "\" could not be found in the current directory." << endl;
		exit(EXIT_FAILURE);
	}
	
	readIn >> cnt; //if cnt==0, return null, catch
	randomIndex = rand()%cnt;
	string tmpStr;
	
	for (int i = 0; getline(readIn,tmpStr); ++i){
		if(i == randomIndex){
			int cmPos = tmpStr.find(",");//Gets position of the comma which delimits IP Address and Port #
			int spPos = tmpStr.find(" ");//Gets position of the space between IP Address and Port #
			destinationHost = make_pair(tmpStr.substr(0, cmPos), tmpStr.substr(spPos+1, (tmpStr.length()-1) -spPos));//Creates the <IP,Port> pair for the destination host
			cout << tmpStr.substr(0, cmPos) << endl;
			cout << tmpStr.substr(spPos+1, (tmpStr.length()-1) -spPos) << endl;
		}//End if
	}//End for
	
	return destinationHost;//return <IP,Port> Pair
}
//End readFile

/* setupConnection simply wraps up the ugliness of getting the hosts IP, creating a socket and binding it to a port.
 * 
 *	PARAMETERS
 *		conn - Contains the <IP,Port> pair for the server this client will connect to and forward the initial wget() command onto.
 *
 *	RETURN
 *		int - Returns the socket file descriptor for the TCP connection b/w this host and the server. The socket file descriptor can then be used in send, write, read etc.
 */
int setupConnection(pair<string,string> conn){
	int socket_fd = -1;
	//Establish a tcp connection here kthxbai.
	return socket_fd;
}
//End setupConnection
