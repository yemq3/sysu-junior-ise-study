import os

path = "."

filename = "ppt.tex"

file = os.path.join(path, filename)

with open(file) as f:
    print(f.readline())