/*
 * Derek Drummond and Sam Bruns
 * PA2 - awget code file
 * Due: 09/12/14
*/

#include "awget.h"

#define DEFAULTFILE "chaingang.txt"

using namespace std;

char * filename = "chaingang.txt";//Default filename as specified by write-up
char * URL = "www.google.com/index.html";//Default URL as specified by write-up

int main(int argc, char* argv[]){

	pair<string,string> dest;

	if(!parseArgs(argc, argv, filename, URL)){
		cerr << "Usage: URL to get or filename of stepping stones to use was not specified.\n awget www.google.com -c stones.txt " << endl;
		exit(EXIT_FAILURE);
	}//End if
	
	dest = selectRandomStep(filename);
	//cout << "<" << get<0>(dest) << ", " << get<1>(dest) << ">" << endl;
	
	
	//This whole If block is probably unnecessary but it catches the case where the chaingang.txt file has no entries...in no way is this anonymous but you still get the file...booyah!
	if(strncmp(get<0>(dest).c_str(), "127.0.0.1", get<0>(dest).length()-1) == 0 && strncmp(get<1>(dest).c_str(), "0", 1) == 0){
		cout << "No stepping stones were provided in the file " << filename << " fetching the content at " << URL << "." << endl;
		//system(wget(URL));
		exit(EXIT_SUCCESS);
	}
	
	
	int socket_fd = setupConnection(dest);
	if(socket_fd == -1){
		cerr << "A server could not be contacted. This most likely means the FBI have seized those systems and are tracking your IP at this very instant. Fly, you fool!" << endl;
		exit(EXIT_FAILURE);
	}
	
	/*After setupConnection has completed we have the socket descriptor associated with a specific port and are ready to communicate with the first server.
	 * There are 2 big questions we need to answer before implementing all of this though.
	 *      1.) How are we sending the list of stepping stones across the connection? 
	 *				A: So far it looks like a stringstream is the best way of transmitting a file. I used the SO topic http://stackoverflow.com/questions/7122254/bad-file-descriptor-with-bsd-socket
	 *					Essentially what this means is we will send each line of the file one packet at a time and reconstruct it into a new file on the server side.
	 *					We don't even have to keep a local copy this way. All that would occur is a stepping stones file would exist in memory and be lost when the program exits.
	 *		2.) Are we going to branch new connections for multiple requests or use the select command?
	 *				A: With the SO topic referenced above it looks like select is nice and simple.
	*/
	
}//End Main


int parseArgs(int argc, char* argv[], char * URL, char* filename){
	
	//I'm probably going to come back and add a lot more thorough error checking which is why I decided to make this a function rather than doing it all in main.
	//Its bad form to assume optional arguments will appear in the given order.
	
	if (argc == 2 || argc == 4){
		URL = argv[1];
		if(argc == 4){
			filename = argv[3];
			return 1;//True
		}
		return 1;
	}
	else return 0;//False
	
}
//End parseArgs

pair<string,string> selectRandomStep(char * filename){
	
	int cnt = 0;
	int randomIndex;
	pair<string,string> destinationHost = make_pair("127.0.0.1", "0");
	ifstream readIn(filename, ifstream::in);
	
	if(!readIn){
		cerr << "The file \"" << filename << "\" could not be found in the current directory." << endl;
		exit(EXIT_FAILURE);
	}
	
	readIn >> cnt;
	randomIndex = rand()%cnt;
	string tmpStr;
	
	for (int i = 0; getline(readIn,tmpStr); ++i){
		if(i == randomIndex){
			int cmPos = tmpStr.find(",");//Gets position of the comma which delimits IP Address and Port #
			int spPos = tmpStr.find(" ");//Gets position of the space between IP Address and Port #
			destinationHost = make_pair(tmpStr.substr(0, cmPos), tmpStr.substr(spPos+1, (tmpStr.length()-1) -spPos));//Creates the <IP,Port> pair for the destination host
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
