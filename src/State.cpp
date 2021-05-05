#include "State.hpp"

#include "GameServer.hpp"

#include <boost/filesystem.hpp>

#include <sstream>
#include <iostream>



std::shared_ptr<Session::State> Session::State_Login::handleCommand(Session &session, Session::Command cmd){
	mongo::client::initialize();

	mongo::DBClientConnection dbConnection;
	
	try{
		dbConnection.connect("localhost");
	} catch (const mongo::DBException &exception){
		std::cout << "Caught " << exception.what() << std::endl;
	}

	try{
		if(!cmd.empty()){
			if(cmd.at(0) == "login"){
				session.m_channel->write("Bem vindo ao MUD!\nUse \"entrar [nome_da_personagem]\" ou \"criar [nome_da_personagem]\"");
			} else if (cmd.at(0) == "entrar"){
				std::string characterName = cmd.at(1);

				std::stringstream ss;
				ss << "Entrando com personagem \"" << characterName << "\"...\n";

				boost::filesystem::path characterPath = session.getGameServer().getCharacterDir() / boost::filesystem::path(characterName);
				if(dbConnection.count("mud.characters", BSON("name" << characterName)) == 0){
					ss << "Erro! Não existe uma personagem com esse nome.";

					session.m_channel->write(ss.str());
					return m_self.lock();
				}

				std::shared_ptr<Character> character(new Character());
				if(character->readFromDatabase(dbConnection, characterName)){
					ss << "Sucesso.";

					session.m_activeCharacter.swap(character);

					session.m_channel->write(ss.str());
					return State::make<State_Game>(&session);
				} else {
					ss << "Erro! Não foi possivel carregar esta personagem.";

					session.m_channel->write(ss.str());
					return m_self.lock();
				}
			} else if (cmd.at(0) == "criar"){
				std::string characterName = cmd.at(1);

				std::stringstream ss;
				ss << "Criando personagem \"" << characterName << "\"...\n";

				if(dbConnection.count("mud.characters", BSON("name" << characterName)) != 0){
					ss << "Erro! Já existe uma personagem com esse nome.";

					session.m_channel->write(ss.str());
					return m_self.lock();
				}

				std::shared_ptr<Character> character(new Character(characterName));
				if(character->writeToDatabase(dbConnection)){
					ss << "Sucesso.";

					session.m_activeCharacter.swap(character);

					session.m_channel->write(ss.str());
					return State::make<State_Game>(&session);
				} else {
					ss << "Erro! Não foi possivel criar a personagem.";

					session.m_channel->write(ss.str());
					return m_self.lock();
				}
			} else {
				session.m_channel->write("Commando inválido. Use \"entrar [nome_da_personagem]\" ou \"criar [nome_da_personagem]\"");
			}
		} else {
			session.m_channel->write("Commando inválido. Use \"entrar [nome_da_personagem]\" ou \"criar [nome_da_personagem]\"");
		}
	} catch (std::out_of_range &e){
			session.m_channel->write("Commando inválido. Use \"entrar [nome_da_personagem]\" ou \"criar [nome_da_personagem]\"");
	}

	return m_self.lock();
};



std::shared_ptr<Session::State> Session::State_Game::handleCommand(Session &session, Session::Command cmd){
	mongo::client::initialize();

	mongo::DBClientConnection dbConnection;
	
	try{
		dbConnection.connect("localhost");
	} catch (const mongo::DBException &exception){
		std::cout << "Caught " << exception.what() << std::endl;
	}

	std::stringstream ss;

	ss << session.m_activeCharacter->getMessages() << std::endl;
	session.m_activeCharacter->clearMessages();

	try{
		if(!cmd.empty()){
			if(cmd.at(0) == "falar"){
				const std::string &msg = cmd.at(1);
				std::stringstream msgss;

				msgss << session.m_activeCharacter->getName() << " diz \"" << msg << "\".";

				for(auto &e : session.getGameServer().m_sessions){
					if(e.second.m_activeCharacter->getPosition() == session.m_activeCharacter->getPosition()){
						e.second.m_activeCharacter->addMessage(msgss.str());
					}
				}

				ss << session.m_activeCharacter->getMessages();
				session.m_activeCharacter->clearMessages();
				
				session.m_channel->write(ss.str());
				return m_self.lock();
			//} else if (cmd.at(0) == "andar"){
			//	const std::string &dir = cmd.at(1);
			} else if (cmd.at(0) == "olhar"){
				ss << session.m_activeCharacter->getLookStr(session.getGameServer().getMap());

				session.m_channel->write(ss.str());
				return m_self.lock();
			} else if (cmd.at(0) == "ranking"){
				std::auto_ptr<mongo::DBClientCursor> cursor = dbConnection.query("mud.characters", mongo::Query().sort("experience"));
				std::cout << dbConnection.getLastErrorDetailed().toString() << std::endl;
				ss << "Ranking de maior experiencia:" << std::endl;
				while (cursor->more()){
					mongo::BSONObj obj = cursor->next();
					ss << "Nome: " << obj.getStringField("name") << "	Level: " << obj.getIntField("level") << "	Experiencia: " << obj.getIntField("experience");
					if(cursor->more()){
						ss << std::endl;
					}
				}

				session.m_channel->write(ss.str());
				return m_self.lock();
			} else {
				ss << "Comando inválido.";

				session.m_channel->write(ss.str());
				return m_self.lock();
			}
		} else {
			ss << "Comando inválido.";

			session.m_channel->write(ss.str());
			return m_self.lock();
		}
	} catch (std::out_of_range &e){
			ss << "Comando inválido.";

			session.m_channel->write(ss.str());
			return m_self.lock();
	}
}
