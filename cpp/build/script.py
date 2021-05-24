import subprocess
import re
import statistics
import os
import math

tests = 5
datasets = ["input1.txt", "input2.txt",
            "input3.txt", "input4.txt", "input5.txt"]
threads = [0, 1, 2, 3]
iterations = [1, 10, 100]

# Parallelism Measuerements
for i in range(len(datasets)):

    print("---------------------------------------------------------------")
    print("---------------------------------------------------------------")

    print("Dataset path:", "\"", datasets[i], "\"")

    for j in range(len(iterations)):

        print("--\Iterations:", iterations[j])

        avg = []
        stddev = []
        speedups = []

        for k in range(len(threads)):

            for m in range(tests):
                command = ""
                # print(command)

                out = subprocess.getoutput(command)
                # print(out)

            values = []
            with open('timeCalculations.txt', 'r+') as f:
                for line in f:
                    if line:
                        values.append(float(line.strip()))

            avrg = "{:.5f}".format(sum(values) / len(values))
            dev = "{:.5f}".format(statistics.stdev(values))

            avg.append(float(avrg))
            stddev.append(float(dev))

            print("--Thread(s) #", threads[k], values,
                  "=> avg: ", avrg, "s, stdev: ", dev, " s")

            os.remove("timeCalculations.txt")

            if threads[k] == 0:
                make = subprocess.getoutput("make")
                # print(make)

        #print("avgs:", avg)
        print("Threads, Speedup(C), Speedup(N), Stdev")

        ########## For all Confs ##########
        for y in range(1, len(avg)):

            speedupClassic = "{:.5f}".format(avg[0]/avg[y])
            speedupN = "{:.5f}".format(avg[y-1]/avg[y])
            sdv = "{:.5f}".format(stddev[y])

            print("Thread", threads[y], "  : ",
                  speedupClassic, " ,", speedupN, " ,", sdv)
        ########## End all Confs ##########


print("Total Runs:", len(datasets) * len(iterations) * len(threads) * tests)


print("Cleaning...")
make = subprocess.getoutput("make clean")
print(make)
