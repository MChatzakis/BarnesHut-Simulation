CC = g++
CC_FLAGS = -std=c++0x -ltbb -o

CPP_MAIN = ./../cpp/src/BarnesHut.cpp
CPP_LIB = ./../cpp/src/BarnesHut.h

JAVA_MAIN =  ./../java/src/simulation/BarnesHutMain.java
JAVA_DIR =  ./../java/src/

CPP_NAME = bhCPP
JAVA_NAME = BarnesHutMain
JAR_NAME = bhJAVA.jar

all: c++ javaBuild 

tests: script.py
	python3 script.py

javaBuild: java cleanJavaClassFiles
	
c++: $(CPP_LIB) $(CPP_MAIN)
	$(CC) $(CPP_MAIN) $(CC_FLAGS) $(CPP_NAME) 

java: $(JAVA_MAIN)
	javac -d ./ $(JAVA_DIR)simulation/*.java $(JAVA_DIR)structures/*.java 
	jar cfe $(JAR_NAME) simulation.BarnesHutMain structures/*.class simulation/*.class

clean: cleanCPP cleanJava

cleanAll: clean
	rm -rf tables.tex

cleanJavaClassFiles:
	rm -rf *.class	
	rm -rf simulation/*.class
	rm -rf simulation
	rm -rf structures/*.class
	rm -rf structures

cleanJava: cleanJavaClassFiles
	rm -rf $(JAR_NAME)	
	rm -rf simulationJAVA.txt
	rm -rf timesJAVA.txt
	
cleanCPP: 
	rm -rf $(CPP_NAME)	
	rm -rf simulationCPP.txt
	rm -rf timesCPP.txt
	