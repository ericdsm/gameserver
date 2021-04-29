#ifndef Server_hpp_HEADERGUARD
#define Server_hpp_HEADERGUARD

#include "CommunicationChannel.hpp"
#include "Connection.hpp"

#include <boost/asio.hpp>

#include <memory>
#include <vector>
#include <functional>


class Server{
	public:
		using OnConnectHandler = std::function<void(Connection&)>;
		using OnMessageHandler = Connection::OnMessageHandler;

		Server(boost::asio::io_service&, boost::asio::ip::tcp::endpoint);

		void start();

		void setOnConnectHandler(OnConnectHandler);
		void setOnMessageHandler(OnMessageHandler);

	private:
		void accept();
		void acceptHandler(const boost::system::error_code&, std::shared_ptr<Connection>);

		boost::asio::io_service &m_ioService;
		boost::asio::ip::tcp::acceptor m_tcpAcceptor;
		std::vector<std::shared_ptr<Connection>> m_connections;

		OnConnectHandler m_onConnectHandler;
		OnMessageHandler m_onMessageHandler;
};


#endif //Server_hpp_HEADERGUARD
