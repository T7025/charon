import re


should_write = False
with open("cmake-build-release/main/info_vec.txt") as f:
    out = open("info_vec_reduced.txt", "w")
    for line in f:
        if re.match(r".*loop at /home/thomas/CLionProjects/charon.new/main.*", line):
            should_write = True
            out.write("\n")
        elif not line.strip():
            should_write = False

        if should_write:
            out.write(line)
