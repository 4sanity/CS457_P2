/*
 * Derek Drummond and Sam Bruns
 * PA2 - awget header file
 * Due: 09/12/14
*/

#ifndef AWGET_HEADER
#define AWGET_HEADER

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

///////////////////////////////////////////////////////////STRUCTURES//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////FUNCTIONS////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*This function will parse the command line arguments passed to awget.
 * Primarily this will determine if the optional -c option was specified. Additionally this 
 * function will copy the URl and filename specified into the correct variables
 *
 * PARAMETERS
 *		argc - Number of arguments specified where argc = 0 is the program name.
 *		argv - Array of arguments stored as a sting
 *		URL - Pointer to return location for the URL specified as a command line argument
 *		filename - Pointer to return location for the filename specified as a command line argument
 *
 *  RETURNS
 *		int - returns 1 if all args are correct and 0 in the event of an error. All error messages are printed in readFile function.
 */
int parseArgs(int argc, char* argv[]);


/* This function chooses opens the file specified in filename in the command line arguments reads the entries and chooses a random <IP,Port> pair to be used in the first hop of the anonymous get.
 *
 * NOTES
 *		This function has already converted the IP and Port number to network byte ordering.
 *
 * PARAMETERS
 *		filename - Specifies the file containing the list of <IP,Port> pairs to choose from.
 *	
 * RETURNS
 *		std::pair<int,int> - the specific IP and port of the next host to connect to.
 *		NULL - The functions returns NULL in the event no <IP Port> pairs were provided in the file given as a parameter.
 */
std::pair<std::string,std::string> selectRandomStep();

/* This wraps up all of the ugliness of setting up a TCP connection in a function and returns the socket file descriptor.
 * The IP and Port number for this host is determined inside this function.
 *
 * PARAMETERS
 *		conn - The IP,Port pairing for the host to connect to.
 *
 * RETURNS
 *		int - the socket file descriptor for the newly setup TCP connection
 */
int setupConnection(std::pair<std::string,std::string> conn);

#endif
