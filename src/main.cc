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
    u_int8_t bmat2[7 * 5] = {
        1, 0, 0, 0, 1,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        1, 0, 0, 0, 1};

    BCSR bcsr_mat(7, 5, bmat);
    BCSR bcsr_mat2(7, 5, bmat2);

    std::cout << bcsr_mat << "\n"
              << bcsr_mat.toDnString() << std::endl;

    bcsr_mat.set(0, 0);
    // std::cout << /*bcsr_mat << "\n" <<*/ bcsr_mat.toDnString() << std::endl;
    bcsr_mat.set(0, 4);
    // std::cout << bcsr_mat.toDnString() << std::endl;
    bcsr_mat.set(2, 3);
    // std::cout << bcsr_mat.toDnString() << std::endl;
    bcsr_mat.set(6, 0);
    bcsr_mat.reset(0, 2);
    bcsr_mat.reset(0, 1);
    std::cout << bcsr_mat << "\n";
    std::cout << bcsr_mat.toDnString() << std::endl;

    bcsr_mat |= bcsr_mat2;
    std::cout << bcsr_mat << "\n";
    std::cout << bcsr_mat.toDnString() << std::endl;
    return 0;
}
