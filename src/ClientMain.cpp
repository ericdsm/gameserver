#include "Client.hpp"

#include <iostream>


void requestCycle(std::shared_ptr<Client> client, std::string response){
	std::cout << "<Response] " << response << std::endl;

	std::cout << "[Input> ";
	std::string req;
	std::getline(std::cin, req);

	client->request(req, std::bind(requestCycle, client, std::placeholders::_1));
}

int main(){
	boost::system::error_code error;

	unsigned int port = 3333;
	std::string address = "127.0.0.1";

	boost::asio::ip::address ip = boost::asio::ip::address::from_string(address, error);
	if(error.value() != 0){
		std::cerr << "Invalid IP address" << address << std::endl;
		return error.value();
	}

	boost::asio::ip::tcp::endpoint endpoint(ip, port);
	boost::asio::io_service ioService;
	boost::asio::io_service::work ioServiceWork(ioService);

	std::shared_ptr<Client> client(new Client(ioService, endpoint));

	client->setOnConnectHandler([&client](){
		client->request("login\x17", std::bind(requestCycle, client, std::placeholders::_1));
	});

	client->connect();
	ioService.run();


	return 0;
}
