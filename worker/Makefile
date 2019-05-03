CXX = clang++
CXXFLAGS = -Wall -O2 -std=c++11
LDFLAGS = -lcurl

all:	worker
worker: worker.cpp
	$(CXX) $(CXXFLAGS) worker.cpp json11/json11.cpp -o worker $(LDFLAGS)
