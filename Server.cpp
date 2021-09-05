
#include "Server.h"

Server::Server(int port)throw (const char*) {

	this->fd = socket(AF_INET,SOCK_STREAM,0);
    if (fd < 0)
      throw "socket failed";
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = INADDR_ANY;
    this->server.sin_port = htons(port);

    if(bind(fd,(struct sockaddr*) &server, sizeof(server)) < 0)
      throw "bind failure";

    if(listen(fd, 3) < 0)
      throw "listen failure";
}

void Server::start(ClientHandler& ch)throw(const char*){	
    
    this->is_server_runinning = true;
        t = new thread([&ch,this]() {
            while(is_server_runinning) {
            cout << "waiting for client" << endl;
            socklen_t clientSize = sizeof(client);
            alarm(1);
            int aClient = accept(fd, (struct sockaddr*) &client, &clientSize);
            alarm(0);
            if(aClient < 0) {
              throw "accept failure";
            }
            cout << "client connected!" << endl;
            ch.handle(aClient);
            close(aClient);
            
            }
            close(fd);
        });
         
};

void Server::stop(){
  this->is_server_runinning = false;
	t->join(); // do not delete this!
}


Server::~Server() {
}

