#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#ifndef NETWORK_H
#define NETWORK_H

/*
Initializing UDP sockets and set to listen to port 12345
intput: void
output: void
*/
void openConnection();


/*
Receives messages sent from client and returns number of characters in the message
input: *char, int
output: int
*/
int receiveRequest(char *request, int strLen);


/*
Sends messege to client. Messages are split into 1024 bytes
and sends them in order to prevent exceeding max packet size.
Returns number of packets sent.
input: *char, bool
output: int 
*/
int sendResponse(char *str);


/*
Close udp connection
input: void
output: void
*/
void closeConnection();

#endif