CC = g++
CC_FLAGS = -std=c++0x -ltbb -o

CPP_MAIN = ./../src/cpp/BarnesHut.cpp
CPP_LIB = ./../src/cpp/BarnesHut.h

JAVA_MAIN =  ./../src/java/BarnesHutMain.java

CPP_NAME = bhCPP
JAVA_NAME = bhJava

all: g++

g++: $(CPP_LIB) $(CPP_MAIN)
	$(CC) $(CPP_MAIN) $(CC_FLAGS) $(CPP_NAME) 

java: $(JAVA_MAIN)
	javac -d ./ $(JAVA_MAIN)

cleanAll: 
	rm -rf $(CPP_NAME)
	rm -rf *.class	
	rm -rf simulationCPP.txt
	rm -rf timeMesCPP.txt
	rm -rf tables.tex

clean: 
	rm -rf $(CPP_NAME)	
	rm -rf simulationCPP.txt
	rm -rf timeMesCPP.txt
