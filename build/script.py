import subprocess
import re
import statistics
import os
import copy
import math

tests = 5
datasets = ["./../datasets/input5.txt","./../datasets/input2.txt"]
# "./../../datasets/input3.txt", "./../../datasets/input4.txt", "./../../datasets/input5.txt"]
threads = [0,1,2,4]
iterations = [30, 100, 200]

tex = open("tables.tex", "w+")

#compile all before start
print("Compiling")
make = subprocess.getoutput("make")
print(make)

# Parallelism Measuerements
tex.write("\subsection*{4.1 C++ Implementation}")
for i in range(len(datasets)):

    print("---------------------------------------------------------------")

    print("Dataset path:", datasets[i])

    tex.write("\subsubsection*{4.1." + str((i+1)) + " " +
              datasets[i] + "} \n \\begin{itemize}\n")

    for j in range(len(iterations)):

        print("Iterations:", iterations[j])

        tex.write(
            "\item " + str(iterations[j]) + " iterations\n\\begin{center}\n \\begin{adjustbox}{width=\columnwidth, center} \n \\begin{tabular}{ | |c | c c c c c | c | c c | c | |} \hline \n Threads & Run 1 & Run 2 & Run 3 & Run 4 & Run 5 & Average & Speedup(C) & Speedup(N) & Stdev \\\\ [0.5ex] \n \hline \n \hline \n")

        avg = []
        vls = []
        stddev = []
        speedups = []

        for k in range(len(threads)):

            for m in range(tests):
                command = "./bhCPP -f " + datasets[i] + " -i " + \
                    str(iterations[j]) + " -t " + str(threads[k]) + " -m"
                print(command)

                out = subprocess.getoutput(command)
                print(out)

            values = []
            with open('timesCPP.txt', 'r+') as f:
                for line in f:
                    if line:
                        values.append(float(line.strip()))

            avrg = "{:.5f}".format(sum(values) / len(values))
            dev = "{:.5f}".format(statistics.stdev(values))

            avg.append(float(avrg))
            vls.append(copy.deepcopy(values))
            stddev.append(float(dev))

            # print("--Thread(s) #", threads[k], values,
            #     "=> avg: ", avrg, "s, stdev: ", dev, " s")

            os.remove("timesCPP.txt")

        # print("avgs:", avg)
        print("Threads, Average ,Speedup(C), Speedup(N), Stdev")

        ########## For all Confs ##########
        for y in range(0, len(avg)):

            speedupClassic = 0
            speedupN = 0
            sdv = 0

            if(y > 0):
                speedupClassic = "{:.5f}".format(avg[0]/avg[y])
                speedupN = "{:.5f}".format(avg[y-1]/avg[y])
                sdv = "{:.5f}".format(stddev[y])

            print("Thread", threads[y], "  : ", vls[y], " ,", avg[y], " ,",
                  speedupClassic, " ,", speedupN, " ,", sdv)

            tex.write(str(threads[y]) + "& ")
            for n in range(len(vls[y])):
                tex.write(str(vls[y][n]) + " & ")

            tex.write(str(avg[y]) + " & " + str(speedupClassic) +
                      " & " + str(speedupN) + " & " + str(sdv) + "\\\\ \n \hline\n")

            ########## End all Confs ##########

        tex.write("\end{tabular} \end{adjustbox} \end{center}")
    tex.write("\end{itemize}")

tex.write("\subsection*{4.2 Java Implementation}")

tex.close()
print("Total Runs:", len(datasets) * len(iterations) * len(threads) * tests)

print("Cleaning...")
make = subprocess.getoutput("make clean")
print(make)
