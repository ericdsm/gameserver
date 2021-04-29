CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
PROJECT(gameserver)


FIND_PACKAGE(Boost REQUIRED COMPONENTS system filesystem)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "-pthread")
SET(SOURCES
	src/Connection.cpp
	src/CommunicationChannel.cpp
	src/Server.cpp
	src/Client.cpp

	src/GameServer.cpp
	src/Session.cpp
	src/State.cpp
	src/Map.cpp
	src/Character.cpp
)


ADD_EXECUTABLE(gameserver ${SOURCES} src/ServerMain.cpp)
TARGET_LINK_LIBRARIES(gameserver ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(gameserver mongoclient)


ADD_EXECUTABLE(gameclient ${SOURCES} src/ClientMain.cpp)
TARGET_LINK_LIBRARIES(gameclient ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(gameclient mongoclient)
