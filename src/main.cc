#include <stdio.h>
#include <iostream>
#include "csr/bcsr.hh"

int main(int argc, char const *argv[])
{
    u_int8_t mat[7][5] = {
        {8,0,2,0,0},
        {0,0,5,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,7,1,2},
        {0,0,0,9,0}
    };
    u_int8_t bmat[7][5] = {
        {1,0,1,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,1,1},
        {0,0,0,1,0}
    };

    

    std::cout << "Starting...\n";
    return 0;
}
