import math

inCsv = []

with open("benchmarks.csv", "r") as f:
    header = f.readline()
    line = header
    while line:
        line = f.readline()
        lineA = line.split(',')
        for i,_ in enumerate(lineA):
            try:
                lineA[i] = float(lineA[i])
            except ValueError:
                pass
        if line:
            inCsv.append(lineA)
# print(header)
# print(inCsv)

# speedup barnesHutTree and barnesHutOffloading
for typeImpl in ["BarnesHutTreeUniverse", "BarnesHutOffloadUniverse"]:
    outCsv = []
    firstTimes = {}
    for line in inCsv:
        if "BarnesHutMultiThreadUniverse1" in line:
            firstTimes[line[2]] = line[3]

    # print(firstTimes)
    for line in inCsv:
        if typeImpl in line[0]:
            outCsv.append([line[2], firstTimes[line[2]] / line[3]])

    # print(csv)
    with open(typeImpl+"Speedup", "w") as f:
        for line in outCsv:
            f.writelines(f"{line[0]},{line[1]}\n")

# speedup bruteForceOffloading
for typeImpl in ["BruteForceOffloadUniverse"]:
    outCsv = []
    firstTimes = {}
    for line in inCsv:
        if "BruteForceMultiThreadUniverse1" in line:
            firstTimes[line[2]] = line[3]

    # print(firstTimes)
    for line in inCsv:
        if typeImpl in line[0]:
            outCsv.append([line[2], firstTimes[line[2]] / line[3]])

    # print(csv)
    with open(typeImpl+"Speedup", "w") as f:
        for line in outCsv:
            f.writelines(f"{line[0]},{line[1]}\n")

# speedup
for typeImpl in ["BarnesHutMultiThread", "BruteForceMultiThread"]:
    outCsvs = {}
    firstTimes = {}
    for line in inCsv:
        if typeImpl in line[0]:
            numThreads = int(line[0][-1])
            # print("numThreads", numThreads)
            if numThreads == 1:
                firstTimes[line[2]] = line[3]
            outCsvs[numThreads] = []

    # print(firstTimes)
    for line in inCsv:
        if typeImpl in line[0]:
            numThreads = int(line[0][-1])
            # print("numThreads", numThreads)
            outCsvs[numThreads].append([line[2], firstTimes[line[2]] / line[3]])

    for numThreads in outCsvs:
        csv = outCsvs[numThreads]
        # print(csv)
        with open(typeImpl+"Speedup"+str(numThreads), "w") as f:
            for line in csv:
                f.writelines(f"{line[0]},{line[1]}\n")

# strong scaling
for typeImpl in ["BarnesHutMultiThread", "BruteForceMultiThread"]:
    outCsvs = {}
    firstTimes = {}
    for line in inCsv:
        if typeImpl in line[0]:
            numThreads = int(line[0][-1])
            # print("numThreads", numThreads)
            if numThreads == 1:
                firstTimes[line[2]] = line[3]
            outCsvs[line[2]] = []

    # print(firstTimes)
    for line in inCsv:
        if typeImpl in line[0]:
            numThreads = int(line[0][-1])
            # print("numThreads", numThreads)
            outCsvs[line[2]].append([numThreads, min(firstTimes[line[2]] / (line[3] * numThreads), 1.0)])

    for numBodies in outCsvs:
        csv = outCsvs[numBodies]
        # print(numBodies, csv)
        with open(typeImpl+"Strong"+str(int(numBodies)), "w") as f:
            for line in csv:
                # print(line)
                f.writelines(f"{line[0]},{line[1]}\n")

# weak scaling
# for typeImpl in ["BarnesHutMultiThread", "BruteForceMultiThread"]:
#     outCsvs = {}
#     firstTimes = {}
#     numBodiesSet = set()
#     numCoresSet = set()
#
#     times = {}
#     for line in inCsv:
#         if typeImpl in line[0]:
#             numThreads = int(line[0][-1])
#             times[numThreads] = {}
#
#     for line in inCsv:
#         if typeImpl in line[0]:
#             numThreads = int(line[0][-1])
#             times[numThreads][line[2]] = line[3]
#
#     coreNums = [key for key in times]
#     coreNums.sort()
#     maxIters = int(min(math.floor(math.log2(coreNums[-1])), len(times[coreNums[0]])))
#     for key in times:
#         print(key, times[key])
#
#     for i in range(maxIters+1):
#         numCores = 2**i
#         print(numCores, times[numCores][])

    #         # print("numThreads", numThreads)
    #         if numThreads == 1:
    #             firstTimes[line[2]] = line[3]
    #         outCsvs[line[2]] = []
    #         numBodiesSet.add(line[2])
    #         numCoresSet.add(numThreads)
    #
    # # numBodiesList.sort(reverse=True)
    # maxCoreNum = 0
    # for i in numCoresSet:
    #     maxCoreNum = max(maxCoreNum, i)
    # print(maxIters)
    # numBodiesList = [x for x in numBodiesSet]
    # numBodiesList.sort()
    # numBodiesList = numBodiesList[-(maxIters+1):]
    # print(numBodiesList)
    # numCoresList = [x for x in numCoresSet]
    # numCoresList.sort()





# for typeImpl in ["BarnesHutMultiThread", "BruteForceMultiThread"]:
#     outCsvs = {}
#     firstTimes = {}
#     for line in inCsv:
#         if typeImpl in line[0]:
#             numThreads = int(line[0][-1])
#             if numThreads in [2**x for x in range(10)]:
#                 print("numThreads", numThreads)
#                 if numThreads == 1:
#                     firstTimes[numThreads] = line[3]
#                 outCsvs[numThreads] = []
#
#     print(firstTimes)
#     for line in inCsv:
#         if typeImpl in line[0]:
#             numThreads = int(line[0][-1])
#             if numThreads in [2**x for x in range(10)]:
#                 print("numThreads", numThreads)
#                 outCsvs[numThreads].append([line[2], min((firstTimes[line[2]*numThreads] / line[3]), 1.0)])
#
#     for numThreads in outCsvs:
#         csv = outCsvs[numThreads]
#         print(csv)
#         with open(typeImpl+"Weak"+str(numThreads), "w") as f:
#             for line in csv:
#                 f.writelines(f"{line[0]},{line[1]}\n")

