with open("1.txt") as f1, open("2.txt") as f2:
    for _ in range(3):
        print(f1.readline() == f2.readline())