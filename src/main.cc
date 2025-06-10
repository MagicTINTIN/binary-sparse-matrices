#include <stdio.h>
#include <iostream>
#include "csr/bcsr.hh"

int main(int argc, char const *argv[])
{
    std::cout << "Starting...\n";
    u_int8_t mat[7 * 5] = {
        8, 0, 2, 0, 0,
        0, 0, 5, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 7, 1, 2,
        0, 0, 0, 9, 0};
    u_int8_t bmat[7 * 5] = {
        1, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 1, 1,
        0, 0, 0, 0, 0,
        0, 0, 0, 1, 0};


    BCSR bcsr_mat(7, 5, bmat);

    std::cout << bcsr_mat << "\n"
              << bcsr_mat.toDnString() << std::endl;

    return 0;
}
