#ifndef GameServer_hpp_HEADERGUARD
#define GameServer_hpp_HEADERGUARD

#include "Connection.hpp"
#include "Server.hpp"
#include "Session.hpp"
#include "Map.hpp"

#include <boost/filesystem.hpp>

#include <map>


class GameServer{
	public:
		static constexpr unsigned short int DEFAULT_PORT=3333;

		GameServer(boost::asio::io_service&, unsigned short int port = DEFAULT_PORT);

		void start();
		void connectHandler(Connection&);
		void createSession(std::shared_ptr<CommunicationChannel>);
		void messageHandler(CommunicationChannel&, std::string);

		boost::filesystem::path getCharacterDir();
		Map& getMap();


		std::map<CommunicationChannel*, Session> m_sessions;
	private:
		unsigned short int m_port;
		boost::asio::ip::address m_ip;
		boost::asio::ip::tcp::endpoint m_endpoint;
		std::unique_ptr<Server> m_server;

		boost::filesystem::path m_characterDir;
		boost::filesystem::path m_mapPath;

		Map m_map;
};


#endif //GameServer_hpp_HEADERGUARD
