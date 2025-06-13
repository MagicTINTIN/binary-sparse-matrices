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
        0, 0, 1, 1, 0,
        0, 1, 0, 0, 1,
        0, 0, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        1, 1, 0, 0, 1};

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

    if (!bcsr_mat.checkOrder())
        printf("ERROR in ordering!\n");

    BCSR res = bcsr_mat | bcsr_mat2;
    std::cout << res << "\n";

    bcsr_mat |= bcsr_mat2;
    std::cout << bcsr_mat << "\n";
    std::cout << bcsr_mat.toDnString() << std::endl;

    if (!bcsr_mat.checkOrder(true))
        printf("ERROR in ordering!\n");

    printf("###################################################\n");

    u_int8_t bmat3[4 * 3] = {
        1, 0, 1,
        0, 0, 0,
        0, 1, 0,
        0, 0, 1};

    BCSR bcsr3(4, 3, bmat3);
    BCSR bcsr3_0(4, 3);
    BCSR bcsr3t = bcsr3.transpose();
    std::cout << bcsr3 << "\n";
    std::cout << bcsr3.toDnString() << "\n\n";
    std::cout << bcsr3t << "\n";
    std::cout << bcsr3.transpose().toDnString() << std::endl;

    printf("###################################################\n- 4 -\n");

    u_int8_t bmat4[5 * 8] = {
        0, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 1, 0, 0, 1, 1, 0,
        0, 1, 0, 1, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0};

    BCSR bcsr4(5, 8, bmat4);
    BCSR bcsr4t(bcsr4.transpose());
    BCSR bcsr4tt(bcsr4t.transpose());
    std::cout << bcsr4 << "\n";
    std::cout << bcsr4t.toCondensedString(',') << "\n";
    std::cout << bcsr4tt.toCondensedString('|') << "\n";
    std::cout << bcsr4.toDnString() << "\n\n";
    std::cout << bcsr4t.toDnString() << std::endl;
    // std::cout << bcsr4.toDnString() << "\n\n";
    // std::cout << bcsr4.transpose().transpose() << "\n";
    // std::cout << bcsr4.transpose().toDnString() << std::endl;


    int A4_p[6] = {0,2,4,7,10,10};
    int A4_j[10] = {2,7,0,7,2,5,6,1,3,5};
    int R4_p[9] = {0};
    int R4_j[10] = {0};
    scipy_tocsc(5,8,A4_p,A4_j,R4_p,R4_j);

    std::cout << "Scipy:\n" << scipy_tostr(8, 10, R4_p, R4_j) << "\n";

    printf("###################################################\n");

    u_int8_t bmat5[4 * 3] = {
        1, 0, 1,
        0, 0, 0,
        0, 1, 0,
        0, 0, 1};

    u_int8_t bmat5diff[4 * 3] = {
        0, 0, 0,
        0, 1, 1,
        1, 1, 0,
        0, 0, 0};

    BCSR bcsr5(4, 3, bmat3);
    BCSR bcsr5diff(4, 3, bmat5diff);
    BCSR bcsr5res = bcsr5 & bcsr5diff;
    std::cout << bcsr5res << "\n";
    std::cout << bcsr5res.toDnString() << "\n\n";

    u_int8_t bmat6[7 * 6] = {
        1, 0, 0, 1, 0, 1,
        0, 0, 0, 0, 1, 0,
        0, 1, 1, 0, 0, 0,
        1, 1, 0, 0, 0, 0,
        0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 1, 0,
        0, 0, 1, 0, 0, 1};

    u_int8_t bmat6bis[7 * 6] = {
        0, 1, 0, 0, 0, 1,
        0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 0,
        0, 1, 0, 0, 1, 0};

    BCSR bcsr6(7, 6, bmat6), bcsr6bis(7, 6, bmat6bis);
    std::cout << bcsr6 << "\n"
              << bcsr6.toDnString() << "\n";
    std::cout << bcsr6bis << "\n"
              << bcsr6bis.toDnString() << "\n";
    bcsr6 &= bcsr6bis;
    std::cout << bcsr6 << "\n"
              << bcsr6.toDnString() << "\n";

    printf("###################################################\n");

    u_int8_t bmat7[4 * 4] = {
        1, 0, 1, 1,
        1, 0, 1, 0,
        0, 0, 0, 0,
        1, 1, 0, 0};

    u_int8_t bmat7bis[4 * 4] = {
        0, 1, 1, 0,
        1, 0, 1, 0,
        1, 1, 0, 0,
        1, 0, 1, 0};

    BCSR bcsr7(4, 4, bmat7), bcsr7bis(4, 4, bmat7bis);

    std::cout << bcsr7 << "\n"
              << bcsr7bis << "\n";
    BCSR bcsr7res(bcsr7 * bcsr7bis);
    std::cout << bcsr7res << "\n"
              << bcsr7res.toDnString() << "\n";

    u_int8_t bmat8[4 * 5] = {
        1, 0, 1, 1, 1,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 0,
        1, 1, 0, 0, 0};

    u_int8_t bmat8bis[5 * 4] = {
        0, 1, 1, 0,
        1, 0, 0, 0,
        1, 0, 0, 0,
        0, 0, 1, 0,
        1, 0, 0, 1};

    BCSR bcsr8(4, 5, bmat8), bcsr8bis(5, 4, bmat8bis);

    std::cout << bcsr8.toCondensedString(',') << "\n"
              << bcsr8bis.toCondensedString(',') << "\n";
    BCSR bcsr8res(bcsr8 * bcsr8bis);
    std::cout << bcsr8res.toCondensedString() << "\n";
    // std::cout << bcsr8res.toDnString() << "\n";

    int A8_p[5] = {0, 4, 5, 5, 7};
    int A8_j[7] = {0, 2, 3, 4, 3, 0, 1};
    int B8_p[6] = {0, 2, 3, 4, 5, 7};
    int B8_j[7] = {1, 2, 0, 0, 2, 0, 3};
    int R8_p[10] = {0};
    int R8_j[10] = {0};

    scipy_csr_matmat_binary(4, 4, A8_p, A8_j, B8_p, B8_j, R8_p, R8_j);
    std::cout << "Scipy:\n" << scipy_tostr(4, 8, R8_p, R8_j) << "\n";

    return 0;
}
