CXX=g++
CXXFLAGS=-g -std=c++11 -static-libstdc++ -Wall -pedantic -Wextra
LINK.o = $(LINK.cpp)

all: ipk-client ipk-server

ipk-client: ipk-client.o

ipk-client.o: ipk-client.cpp 

ipk-server: ipk-server.o 

ipk-server.o: ipk-server.cpp

clean:
	rm -f *.o ipk-client ipk-server

