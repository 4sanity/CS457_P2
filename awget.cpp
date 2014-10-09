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

	}//End of
		
	dest = selectRandomStep();
	
	int clientSocket;
	struct sockaddr_in serverAddress;
	if((clientSocket=socket(PF_INET,SOCK_STREAM,0))<0){
		cout << "Error: Client socket could not be created." << endl;
		exit(1);
	}	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(inet_network(get<0>(dest).c_str()));  //inet_addr(SADDRESS)
	serverAddress.sin_port = htons(atoi(get<1>(dest).c_str()));
	if((connect(clientSocket,(struct sockaddr *)&serverAddress,sizeof(sockaddr_in))< 0)){
		cout << "Error: Failed to connect with server." << endl;
		exit(1);
	}
	cout << "Connection successful... Sending files and waiting for return." << endl;
	//strip from chaingang here
	
	while(true){
		char buff[1000];
		strcpy(buff,URL);
		send(clientSocket,buff,strlen(URL),0);
		recv(clientSocket,buff,strlen(buff),0);
		exit(1);
		//printf("Friend: %s\n",packet.message);
	}
	
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
	
	readIn >> cnt;
	string newline;
	getline(readIn,newline);
	randomIndex = rand()%cnt+1; //random index between 1 and cnt
	string tmpStr;
	
	for (int i = 1; i<=cnt; i++){
		getline(readIn,tmpStr);
		if(i == randomIndex){
			int cmPos = tmpStr.find(",");//Gets position of the comma which delimits IP Address and Port #
			int spPos = tmpStr.find(" ");//Gets position of the space between IP Address and Port #
			destinationHost.first = tmpStr.substr(0, cmPos);
			destinationHost.second = tmpStr.substr(spPos+1,(tmpStr.length()-1)-spPos);
		}//End if
	}//End for
	
	return destinationHost;//return <IP,Port> Pair
}
//End readFile
