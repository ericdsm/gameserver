#include "Session.hpp"

#include "State.hpp"

#include <boost/tokenizer.hpp>

#include <vector>
#include <regex>


Session::Command tokenizeCommandString(std::string str){
	std::regex regex("\"[^\"]*\"|[\\S]+");
	std::sregex_token_iterator i(str.begin(), str.end(), regex);

	std::vector<std::string> result;

	std::sregex_token_iterator end;
	while(i != end){
		const std::string str(*i);
		if(str.front() == '\"' && str.back() == '\"'){
			//Remove quotes
			result.emplace_back(str.substr(1, str.size()-2));
		} else {
			result.emplace_back(str);
		}
		++i;
	}

	return result;
}

Session::Session(GameServer &gameServer, std::shared_ptr<CommunicationChannel> channel):
m_gameServer(gameServer),
m_channel(channel),
m_currentState(State::make<State_Login>(this))
{}

void Session::handleRequest(std::string req){
	Command cmd(tokenizeCommandString(req));
	m_currentState = m_currentState->handleCommand(*this, cmd);
}

GameServer& Session::getGameServer(){
	return m_gameServer;
}
