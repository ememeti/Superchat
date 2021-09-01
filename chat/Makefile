CXX=g++

CXXFLAGS=-Wall -O0 -g -std=c++11

CPPFLAGS=-I/home/bdavis/asio_install/include/
#CPPFLAGS=-I/home/phillip/dwn/asio-1.12.2/include/



all:chat_client chat_server

chat_client.o: chat_client.cpp chat_message.hpp

chat_server.o: chat_server.cpp chat_message.hpp


interface.o: interface.cpp interface.hpp


chat_client: chat_client.o interface.o
	${CXX} -o chat_client chat_client.o interface.o -lpthread -lncurses

chat_server: chat_server.o
	${CXX} -o chat_server chat_server.o -lpthread

clean:
	-rm -f chat_server chat_client chat_server.o chat_client.o
