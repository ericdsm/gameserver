#include "CommunicationChannel.hpp"

#include <iostream>


const std::string CommunicationChannel::MESSAGE_SEPARATOR = "\x17"; //End of transmision block ASCII character

CommunicationChannel::CommunicationChannel(std::shared_ptr<boost::asio::ip::tcp::socket> tcpSocket):
m_tcpSocket(tcpSocket)
{}

void CommunicationChannel::read(){
	std::shared_ptr<boost::asio::streambuf> buffer(new boost::asio::streambuf());

	boost::asio::async_read_until(*m_tcpSocket, *buffer, MESSAGE_SEPARATOR, std::bind(&CommunicationChannel::readHandler, this, std::placeholders::_1, buffer));
}

void CommunicationChannel::write(std::string message){
	std::shared_ptr<boost::asio::streambuf> buffer(new boost::asio::streambuf());
	std::ostream outputStream(buffer.get());

	outputStream << message << MESSAGE_SEPARATOR;

	boost::asio::async_write(*m_tcpSocket, *buffer, std::bind(&CommunicationChannel::writeHandler, this, std::placeholders::_1, buffer));
}

void CommunicationChannel::setOnReadHandler(CommunicationChannel::OnReadHandler h){
	m_onReadHandler = h;
}

void CommunicationChannel::setOnWriteHandler(CommunicationChannel::OnWriteHandler h){
	m_onWriteHandler = h;
}

void CommunicationChannel::readHandler(boost::system::error_code errorCode, std::shared_ptr<boost::asio::streambuf> buffer){
	if(errorCode){
		if(errorCode == boost::asio::error::eof){ //Socket disconnected
			//@TODO: Write this
			return;
		}

		//Error occured
		//@TODO: Error handling
		return;
	}

	std::istream is(buffer.get());
	std::stringstream ss;

	while(is){
		std::string line;
		std::getline(is, line);

		ss << line << std::endl;
	}

	std::string res = ss.str();

	auto pos = res.find(MESSAGE_SEPARATOR);

	if(pos != std::string::npos){
		res = res.substr(0, pos);
	}

	if(m_onReadHandler){
		m_onReadHandler(res);
	}

	//@TODO: Implement this

	
}

void CommunicationChannel::writeHandler(boost::system::error_code errorCode, std::shared_ptr<boost::asio::streambuf> buffer){
	if(errorCode){
		//@TODO: Error handling
		return;
	}

	if(m_onWriteHandler){
		m_onWriteHandler();
	}
}
