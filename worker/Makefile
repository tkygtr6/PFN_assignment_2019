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

update_active_jobs.o:		update_active_jobs.cpp types.hpp
	$(CXX) $(CXXFLAGS) -c update_active_jobs.cpp -o update_active_jobs.o $(LDFLAGS)

add_new_jobs.o:		add_new_jobs.cpp types.hpp parser.cpp
	$(CXX) $(CXXFLAGS) -c add_new_jobs.cpp -o add_new_jobs.o $(LDFLAGS)

activate_jobs.o:	activate_jobs.cpp types.hpp
	$(CXX) $(CXXFLAGS) -c activate_jobs.cpp -o activate_jobs.o $(LDFLAGS)

worker: 	main.cpp parser.o json11.o init.o update_active_jobs.o add_new_jobs.o activate_jobs.o types.hpp
	$(CXX) $(CXXFLAGS) main.cpp parser.o json11.o init.o update_active_jobs.o add_new_jobs.o activate_jobs.o -o worker -lcurl $(LDFLAGS)

clean:
	rm -rf *.o worker worker.dSYM
