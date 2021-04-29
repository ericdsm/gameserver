#include "Server.hpp"


Server::Server(boost::asio::io_service &ios, boost::asio::ip::tcp::endpoint ep):
m_ioService(ios),
m_tcpAcceptor(ios, ep)
{}

void Server::setOnConnectHandler(OnConnectHandler h){
	m_onConnectHandler = h;
}

void Server::setOnMessageHandler(OnMessageHandler h){
	m_onMessageHandler = h;

}

void Server::start(){
	accept();
}

void Server::accept(){
	std::shared_ptr<Connection> conn(new Connection(m_ioService));

	m_tcpAcceptor.async_accept(conn->getSocket(), std::bind(&Server::acceptHandler, this, std::placeholders::_1, conn));
}

void Server::acceptHandler(const boost::system::error_code &errorCode, std::shared_ptr<Connection> conn){
	if(!errorCode){ //Successful connection
		if(m_onConnectHandler){
			m_onConnectHandler(*conn);
		}

		if(m_onMessageHandler){
			conn->setOnMessageHandler(m_onMessageHandler);
		}

		conn->start();

		m_connections.push_back(conn);
	}

	accept();
}
