#include "GameServer.hpp"

#include <iostream>


int main(int argc, char *argv[]){
	boost::asio::io_service ioService;
	boost::asio::io_service::work work(ioService);

	GameServer server(ioService);
	server.start();

	ioService.run();

	return 0;
};
