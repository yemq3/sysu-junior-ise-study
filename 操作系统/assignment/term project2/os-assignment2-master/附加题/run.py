from matplotlib import pyplot as plt
import numpy as np

n = 0
pageNumbers = []

with open("./out.txt") as f:
    for lines in f:
        address = int(lines, 16)
        offset = address & 0xfff
        pageNumber = (address >> 12) & 0xffff
        pageNumbers.append(pageNumber)
        n += 1
        print(pageNumber)
        if n == 10000:
            break


plt.scatter(range(10000),pageNumbers)
plt.show()
