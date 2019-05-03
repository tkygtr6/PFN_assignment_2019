CXX = clang++
CXXFLAGS = -Wall -O2 -std=c++11
LDFLAGS =

all:		worker

json11.o:	json11/json11.cpp
	$(CXX) $(CXXFLAGS) -c json11/json11.cpp $(LDFLAGS)

parse.o:	parser.cpp
	$(CXX) $(CXXFLAGS) -c parser.cpp -o parser.o $(LDFLAGS)

worker: 	main.cpp parser.o json11.o
	$(CXX) $(CXXFLAGS) main.cpp parser.o json11.o -o worker -lcurl $(LDFLAGS)

clean:
	rm -f *.o worker
