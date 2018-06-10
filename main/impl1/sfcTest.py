import operator

points = [
    (5, 5),
    (3, 0),
    (7, 1),
    (7, 2),
    (8, 0),
    (2, 8),
    (0, 1),
    # (10000000000000000000, 10000000000000000000),
]

for i in range(len(points)):
    points[i] = list(points[i])

# D = 8
k = 16

# calc min & max values in points
minVal = [float("inf") for _ in points[0]]
maxVal = [-float("inf") for _ in points[0]]
for p in points:
    for i in range(len(p)):
        minVal[i] = min(minVal[i], p[i])
        maxVal[i] = max(maxVal[i], p[i])

# move points so bottom left down corner is 0,0,0
for p in points:
    for i in range(len(p)):
        p[i] -= minVal[i]

D = [x - y for x, y in zip(maxVal, minVal)]
print("D:", D)

sfcIndices = []
for pIndex, p in enumerate(points):
    coordinates = [int(2**k * (p[i] / D[i])) - (p[i] == D[i]) for i in range(len(p))]
    print("point:", p)
    print("coord:", coordinates)
    res = 0
    for i in range(k):
        for cIndex, c in enumerate(coordinates):
            res |= (c & (1 << i)) << ((len(coordinates) - cIndex - 1) + i * (len(p) - 1))
        # res |= (coordinates[0] & (1 << i)) << 2 + i * 2
        # res |= (coordinates[1] & (1 << i)) << 1 + i * 2
        # res |= (coordinates[2] & (1 << i)) << 0 + i * 2
        # print(f"{res:b}")
    print(f"result: {res:0{k*len(p)}b}")
    sfcIndices.append((res, 0, p))

print(sfcIndices)
sfcIndices.sort()
print(sfcIndices)
size = len(sfcIndices)
for index, (sfc, size, p) in enumerate([sfcIndices[i] for i in range(size)]):
    print(f"sfc: {sfc:0{k*len(p)}b}")
    if index == len(sfcIndices) - 1:
        break
    sfcNext = sfcIndices[index + 1][0]

    xorVal = sfc ^ sfcNext
    print(f"xor: {xorVal:0{k*len(p)}b}")
    t = (xorVal.bit_length() + len(p) - 1) // len(p)
    # print(t)
    # print(2 ** t)
    # print(2 ** (t - k))
    sfcIndices.append((sfc | ~0 >> t * len(p), -t, [-float("inf") for _ in p]))
    #sfcIndices.append((sfc >> (t * len(p)), -t, [-float("inf") for _ in p]))

l = len(points[0])
for sfc, size, p, in sfcIndices:
    print(f"{sfc:0{k*l}b}, size: {size:3d}, point: {p}")

sfcIndices = sfcIndices[0:len(points)] + sorted(sfcIndices[len(points):]) #, key=operator.itemgetter(0, 1))
print()

for sfc, size, p, in sfcIndices:
    print(f"{sfc:0{k*l}b}, size: {size:3d}, point: {p}")

sfcIndices = sfcIndices[0:len(points)] + [x for i, x in enumerate(sfcIndices[len(points):]) if (
    sfcIndices[i-1 + len(points)] != sfcIndices[i + len(points)] if i else True
)]

for sfc, size, p, in sfcIndices:
    print(f"{sfc:0{k*l}b}, size: {size:3d}, point: {p}")

sfcIndices.sort(key=lambda x: x[0])
print()
for sfc, size, p, in sfcIndices:
    print(f"{sfc:0{k*l}b}, size: {size:3d}, point: {p}")
# for p in points:
#     print("point", p)
#     coordinates = (2**k * p[0] // D, 2**k * p[1] // D, 2**k * p[2] // D)
#     print("coord", coordinates)
#     for c in coordinates:
#         print(f"c: {c:b}")
#     res = 0
#     for i in range(k):
#         res |= (coordinates[0] & (1 << i)) << 2 + i * 2
#         res |= (coordinates[1] & (1 << i)) << 1 + i * 2
#         res |= (coordinates[2] & (1 << i)) << 0 + i * 2
#         print(f"{res:b}")
#
#     print(f"res: {res:b}")
