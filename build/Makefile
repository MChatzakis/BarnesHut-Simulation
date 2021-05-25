CC = g++
CC_FLAGS = -std=c++0x -ltbb -o

CPP_MAIN = ./../src/cpp/BarnesHut.cpp
CPP_LIB = ./../src/cpp/BarnesHut.h

JAVA_MAIN =  ./../src/java/simulation/BarnesHutMain.java
JAVA_DIR =  ./../src/java/

CPP_NAME = bhCPP
JAVA_NAME = BarnesHutMain

all: g++ java

g++: $(CPP_LIB) $(CPP_MAIN)
	$(CC) $(CPP_MAIN) $(CC_FLAGS) $(CPP_NAME) 

java: $(JAVA_MAIN)
	javac -d ./ $(JAVA_DIR)simulation/*.java $(JAVA_DIR)structures/*.java 

cleanAll: 
	rm -rf $(CPP_NAME)
	
	rm -rf simulationCPP.txt
	rm -rf timeMesCPP.txt
	rm -rf tables.tex

clean: 
	rm -rf $(CPP_NAME)	
	rm -rf simulationCPP.txt
	rm -rf timeMesCPP.txt
	rm -rf *.class	
	rm -rf simulation/*.class
	rm -rf simulation
	rm -rf structures/*.class
	rm -rf structures