#include "Client.hpp"


Client::Client(boost::asio::io_service &ioService, boost::asio::ip::tcp::endpoint endpoint):
m_tcpSocket(new boost::asio::ip::tcp::socket(ioService)),
m_endpoint(endpoint),
m_communicationChannel(new CommunicationChannel(m_tcpSocket))
{}

void Client::connect(){
	m_tcpSocket->async_connect(m_endpoint, std::bind(&Client::handleConnect, this, std::placeholders::_1));
}

void Client::request(std::string request, ResponseHandler handler){
	auto self = this; //@TODO: See if I can get rid of this

	m_communicationChannel->setOnWriteHandler([self](){
		self->m_communicationChannel->read();
	});

	m_communicationChannel->setOnReadHandler([handler](std::string response){
		handler(response);	
	});

	m_communicationChannel->write(request);
}

void Client::setOnConnectHandler(OnConnectHandler h){
	m_onConnectHandler = h;
}

boost::asio::ip::tcp::socket& Client::getSocket(){
	return *m_tcpSocket;
}

bool Client::isConnected(){
		return m_connected;
}

void Client::handleConnect(boost::system::error_code errorCode){
	if(errorCode){
		//@TODO: Error handling
		return;
	}
	m_connected = true;
	if(m_onConnectHandler){
		m_onConnectHandler();
	}
}
