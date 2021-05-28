# BarnesHut-Simulation
Implementing and parallelizing Barnes-Hut algorithm fot N-body simulation

## Prerequisites
To compile and run this program, the system must have the following prerequisites: //
[1]: Java Runtime Enviroment (Java 8 or older) //
[2]: g++ compiler (The program in not tested using clang/llvm) //
[3]: TBB API Support //

## Compilation
To compile:
```
cd build
make
```
The above will compile both C++ and Java implementation //
To compile the implementations individually use:
```
cd build
make c++ //for C++ implementation
```
and 
```
cd build
make java //for Java implementation
```
## Automated testing and statistics calculation:
To calculate the statistics (Python 3 required):
```
cd build
make tests
```
The script will print the output of the C++ and Java applications, their running times and it will
also produce a .tex file, containing the tables used for the latex report.

## Configure and Run
To configure and run for C++ (after compilation):
```
./bhCPP     -f [<string> dataset]
            -i [<int> iterations]
            -t [<int> threads]
            -m Set this to calculate execution time 

eg: ./bhCPP -f ../datasets/input1.txt -i 1000 -t 2 -m      
```
To configure and run for Java (after compilation):
```
java -jar bhJAVA.jar    [<string> dataset]
                        [<int> iterations]
                        [<int> threads]
                        "stream" when you write "stream" as the forth argument, the parallel stream of Java 8 will be executed 

eg: java -jar bhJAVA ../datasets/input1.txt 1000 2
```

## Output
The executables produces two output files: "simulation{LANGUAGE}.csv" (the final coordinates and speeds) and "times{LANGUAGE}.txt" (the time measurements used by the test script for the two implementation)

## Report
There is a report provided, presenting the running times of the implementations on the given datasets for different configurations