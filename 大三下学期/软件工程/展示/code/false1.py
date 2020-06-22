path = "."

filename = "ppt.tex"

file = path + "\\" + filename

with open(file) as f:
    print(f.readline())