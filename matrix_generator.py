#!/usr/bin/env python3
import random

def main():
    height = int(input("Matrix height: "))
    width = int(input("Matrix width: "))
    p = float(input("1-value probability : "))

    data = [
        1 if random.random() < p else 0
        for _ in range(height * width)
    ]

    print(f"u_int8_t mat[{height}*{width}] = {{")
    
    for i in range(height):
        row = data[i*width:(i+1)*width]
        print("    " + ", ".join(str(v) for v in row) + ("," if i < height - 1 else ""))
    
    print("};")

if __name__ == "__main__":
    main()
