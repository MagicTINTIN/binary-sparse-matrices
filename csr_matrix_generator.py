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

    # data = [
    #     1 if random.random() < p else 0
    #     for _ in range(height * width)
    # ]
    pointers = [0]
    columns = []
    nnz=0
    for i in range(height):
        col = -1
        while col < width:
            col += random.randint(1, int(1/p))
            if col < width:
                nnz += 1
                columns.append(col)
        pointers.append(nnz)
        
    # print(f"u_int32_t mat_p[{len(pointers)}] = {{" + ", ".join(str(v) for v in pointers) + "};")
    # print(f"u_int32_t mat_j[{len(columns)}] = {{" + ", ".join(str(v) for v in columns) + "};")
    
    print(f"u_int32_t mat_p[{len(pointers)}] = {{{pointers[0]}")
    for v in range(1,len(pointers)):
        print(f", {pointers[v]}")
    print("};")
    
    
    print(f"u_int32_t mat_p[{len(columns)}] = {{{columns[0]}")
    for v in range(1,len(columns)):
        print(f", {columns[v]}")
    print("};")

if __name__ == "__main__":
    main()
