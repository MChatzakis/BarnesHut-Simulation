CC = g++
CC_FLAGS = -std=c++0x -pthread -ltbb -o

MAIN = ./../source/BarnesHut.cpp
LIB = ./../source/BarnesHut.h

NAME = bhCPP

all: g++

g++: $(LIB)
	$(CC) $(MAIN) $(CC_FLAGS) $(NAME) 

cleanAll: 
	rm -rf $(NAME)	
	rm -rf simulationCPP.txt
	rm -rf timeMesCPP.txt
	rm -rf tables.tex

clean: 
	rm -rf $(NAME)	
	rm -rf simulationCPP.txt
	rm -rf timeMesCPP.txt
