CC = g++
CC_FLAGS = -std=c++0x -pthread -ltbb -o 

MAIN = ./../source/BarnesHut.cpp
LIB = ./../source/BarnesHut.h

NAME = bh

all: g++

g++: $(LIB)
	$(CC) $(MAIN) $(CC_FLAGS) $(NAME) 

clean: 
	rm -rf $(NAME)	
	rm -rf simulationCPP.txt
	rm -rf timeMesCPP.txt