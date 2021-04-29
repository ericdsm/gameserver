# gameserver

Para compilação do projeto são necessários g++ e CMake bem como as bibliotecas Boost e libmongoclient.
Execute "cmake .." no diretório build e rode os executáveis compilados de dentro do diretório build:

cd gameserver/build
cmake ..
make
./gameserver



Em outro terminal:


	cd gameserver/build
	./gameclient


Game client se conectará ao gameserver em localhost
O gameserver se conectará a um banco de dados mongodb em localhost
