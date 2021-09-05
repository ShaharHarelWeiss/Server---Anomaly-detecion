

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "CLI.h"


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file

class SocketIO:public DefaultIO {
	int clientID;
	public:
		SocketIO(int clientID) { 
			this->clientID = clientID;
		}
		~SocketIO() {

		};
		virtual string read() {
			string serverInput="";
			char c = 0;
			recv(clientID, &c, sizeof(char), 0);
			while(c!='\n') {				
				serverInput+=c;
				recv(clientID,&c,sizeof(char) , 0);
			}
	 	return serverInput;
		}
		virtual void write(string text) {
			send(clientID, text.c_str(),text.length(), 0);
			
		}
		virtual void write(float f) {
			//send(clientID, &f , sizeof(f), 0);
		}
		virtual void read(float* f) { return; }

};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{ 
	public:
    virtual void handle(int clientID){
		SocketIO dio(clientID);
		CLI cli(&dio);
		cli.start();
    }
};


// implement on Server.cpp
class Server {
	int fd;
  	sockaddr_in server;
 	sockaddr_in client;
	thread* t; // the thread to run the start() method in
	bool is_server_runinning = false;

	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
