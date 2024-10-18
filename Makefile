CXX = g++
FLAGS =  -g -w

TARGET = exec

#Build rule

$(TARGET): agent.o map.o final.o
	$(CXX) $(FLAGS) -o $(TARGET) agent.o map.o final.o

agent.o: agent.h all_header.h agent.cpp
	$(CXX) $(FLAGS) -c agent.cpp

map.o: map.h all_header.h map.cpp
	$(CXX) $(FLAGS) -c map.cpp

final.o: all_header.h final.cpp
	$(CXX) $(FLAGS) -c final.cpp
