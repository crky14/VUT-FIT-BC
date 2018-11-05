CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic -Wextra
LDFLAGS=-lpcap
LINK.o = $(LINK.cpp)

all: dns-export

dns-export: dns-export.o functions.o
	$(CXX) $(CXXFLAGS) dns-export.o -o dns-export $(LDFLAGS)

dns-export.o: dns-export.cpp functions.h

functions.o: functions.cpp functions.h

clean:
	rm -f *.o dns-export

