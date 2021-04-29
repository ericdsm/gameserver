#ifndef Session_hpp_HEADERGUARD
#define Session_hpp_HEADERGUARD

#include "CommunicationChannel.hpp"

#include "Character.hpp"

#include <memory>
#include <string>


class GameServer;

class Session{
	class State;
	class State_Login;
	class State_Game;

	public:
		using Command = std::vector<std::string>;

		Session(GameServer&, std::shared_ptr<CommunicationChannel>);
		void handleRequest(std::string req);
		bool isLoggedIn();

		GameServer& getGameServer();
	private:
		GameServer &m_gameServer;
		std::shared_ptr<CommunicationChannel> m_channel;

		std::shared_ptr<State> m_currentState;

		std::shared_ptr<Character> m_activeCharacter;
};


#endif //Session_hpp_HEADERGUARD
