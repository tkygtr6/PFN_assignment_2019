CXX = clang++ -g
CXXFLAGS = -Wall -O2 -std=c++11
LDFLAGS =

all:		worker

json11.o:	json11/json11.cpp
	$(CXX) $(CXXFLAGS) -c json11/json11.cpp $(LDFLAGS)

parse.o:	parser.cpp
	$(CXX) $(CXXFLAGS) -c parser.cpp -o parser.o $(LDFLAGS)

init.o:		init.cpp
	$(CXX) $(CXXFLAGS) -c init.cpp -o init.o $(LDFLAGS)

worker: 	main.cpp parser.o json11.o init.o types.hpp
	$(CXX) $(CXXFLAGS) main.cpp parser.o json11.o init.o -o worker -lcurl $(LDFLAGS)

clean:
	rm -rf *.o worker worker.dSYM
