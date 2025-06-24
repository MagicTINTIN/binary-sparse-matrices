#!/usr/bin/env python3
import random
import sys

def main():
    height = 0
    height = 0
    p = 0.
    oneliner = ""
    if len(sys.argv) > 1:
        if len(sys.argv) != 4: #sys.argv[1] == "-h" or sys.argv[1] == "--help" or 
            print(f"You can use params like this: {sys.argv[0]} <height> <width> <proba>")
            exit(0)
        else:
            height = int(sys.argv[1])
            width = int(sys.argv[2])
            p = float(sys.argv[3])
    else:
        height = int(input("Matrix height: "))
        width = int(input("Matrix width: "))
        p = float(input("1-value probability : "))
        oneliner = input("one-liner (-/no) : ")

    data = [
        1 if random.random() < p else 0
        for _ in range(height * width)
    ]

    
    if oneliner == "no":  
        print(f"u_int8_t mat[{height}*{width}] = {{")
        for i in range(height):
            row = data[i*width:(i+1)*width]
            print("    " + ", ".join(str(v) for v in row) + ("," if i < height - 1 else ""))
    else:
        print(f"u_int8_t mat[{height}*{width}] = {{" + ", ".join(str(v) for v in data), end="")
    
    print("};")

if __name__ == "__main__":
    main()
