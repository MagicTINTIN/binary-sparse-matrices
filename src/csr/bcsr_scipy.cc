#include "bcsr.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

void scipy_canonicalize(const u_int32_t n_row,
                        u_int32_t Ap[],
                        u_int32_t Aj[])
{
    for (size_t r = 0; r < n_row; r++)
    {
        std::sort(Aj + Ap[r], Aj + Ap[r + 1]);
    }
}

void scipy_canonicalize(const u_int32_t n_row,
                        std::vector<u_int32_t> &Ap,
                        std::vector<u_int32_t> &Aj)
{
    for (size_t r = 0; r < n_row; r++)
    {
        std::sort(Aj.begin() + Ap[r], Aj.begin() + Ap[r + 1]);
    }
}

void scipy_tocsc(const u_int32_t n_row,
                 const u_int32_t n_col,
                 const u_int32_t Ap[],
                 const u_int32_t Aj[],
                 //    const char Ax[],
                 u_int32_t Bp[],
                 u_int32_t Bi[] //,
                                //  char Bx[]
)
{
    const u_int32_t nnz = Ap[n_row];

    // compute number of non-zero entries per column of A
    std::fill(Bp, Bp + n_col, 0);

    for (u_int32_t n = 0; n < nnz; n++)
    {
        Bp[Aj[n]]++;
    }

    // cumsum the nnz per column to get Bp[]
    for (u_int32_t col = 0, cumsum = 0; col < n_col; col++)
    {
        u_int32_t temp = Bp[col];
        Bp[col] = cumsum;
        cumsum += temp;
    }
    Bp[n_col] = nnz;

    for (u_int32_t row = 0; row < n_row; row++)
    {
        for (u_int32_t jj = Ap[row]; jj < Ap[row + 1]; jj++)
        {
            u_int32_t col = Aj[jj];
            u_int32_t dest = Bp[col];

            Bi[dest] = row;
            // Bx[dest] = Ax[jj];

            Bp[col]++;
        }
    }

    for (u_int32_t col = 0, last = 0; col <= n_col; col++)
    {
        u_int32_t temp = Bp[col];
        Bp[col] = last;
        last = temp;
    }
}

void my_scipy_tocsc(const u_int32_t n_row,
                    const u_int32_t n_col,
                    const std::vector<u_int32_t> &Ap,
                    const std::vector<u_int32_t> &Aj,
                    //    const char Ax[],
                    std::vector<u_int32_t> &Bp,
                    std::vector<u_int32_t> &Bi //,
                                               //  char Bx[]
)
{
    const u_int32_t nnz = Ap[n_row];

    // compute number of non-zero entries per column of A
    // std::fill(Bp, 0);

    // for (u_int32_t n = 0; n < nnz; n++)
    // {
    //     Bp[Aj[n] + 1]++;
    // }

    for (u_int32_t n = 0; n < nnz; n++)
    {
        Bp[Aj[n]]++;
    }

    // cumsum the nnz per column to get Bp[]
    // for (u_int32_t col = 1, prevSum = Bp[0]; col <= n_col; col++)
    // {
    //     prevSum += Bp[col];
    //     Bp[col] = prevSum;
    // }
    for (u_int32_t col = 0, cumsum = 0; col < n_col; col++)
    {
        u_int32_t temp = Bp[col];
        Bp[col] = cumsum;
        cumsum += temp;
    }
    Bp[n_col] = nnz;

    for (u_int32_t row = 0; row < n_row; row++)
    {
        for (u_int32_t jj = Ap[row]; jj < Ap[row + 1]; jj++)
        {
            u_int32_t col = Aj[jj];
            u_int32_t dest = Bp[col];

            Bi[dest] = row;
            // Bx[dest] = Ax[jj];

            Bp[col]++;
        }
    }

    // for (u_int32_t col = n_col; col > 0; col--)
    // {
    //     Bp[col] = Bp[col - 1];
    // }
    // Bp[0] = 0;
    for (u_int32_t col = 0, last = 0; col <= n_col; col++)
    {
        u_int32_t temp = Bp[col];
        Bp[col] = last;
        last = temp;
    }
}

void scipy_csr_matmat(const u_int32_t n_row,
                      const u_int32_t n_col,
                      const u_int32_t Ap[],
                      const u_int32_t Aj[],
                      const char Ax[],
                      const u_int32_t Bp[],
                      const u_int32_t Bj[],
                      const char Bx[],
                      u_int32_t Cp[],
                      u_int32_t Cj[],
                      char Cx[])
{
    std::vector<u_int32_t> next(n_col, -1);
    std::vector<char> sums(n_col, 0);

    u_int32_t nnz = 0;

    Cp[0] = 0;

    for (u_int32_t i = 0; i < n_row; i++)
    {
        u_int32_t head = -2;
        u_int32_t length = 0;

        u_int32_t jj_start = Ap[i];
        u_int32_t jj_end = Ap[i + 1];
        for (u_int32_t jj = jj_start; jj < jj_end; jj++)
        {
            u_int32_t j = Aj[jj];
            char v = Ax[jj];

            u_int32_t kk_start = Bp[j];
            u_int32_t kk_end = Bp[j + 1];
            for (u_int32_t kk = kk_start; kk < kk_end; kk++)
            {
                u_int32_t k = Bj[kk];

                sums[k] += v * Bx[kk];

                if (next[k] == -1)
                {
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (u_int32_t jj = 0; jj < length; jj++)
        {

            if (sums[head] != 0)
            {
                Cj[nnz] = head;
                Cx[nnz] = sums[head];
                nnz++;
            }

            u_int32_t temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
            sums[temp] = 0;
        }

        Cp[i + 1] = nnz;
    }
}

void scipy_csr_matmat(
    const u_int32_t n_row,
    const u_int32_t n_col,
    const std::vector<u_int32_t> &Ap,
    const std::vector<u_int32_t> &Aj,
    const char Ax[],
    const std::vector<u_int32_t> &Bp,
    const std::vector<u_int32_t> &Bj,
    const char Bx[],
    std::vector<u_int32_t> &Cp,
    std::vector<u_int32_t> &Cj,
    char Cx[])
{
    std::vector<u_int32_t> next(n_col, -1);
    std::vector<char> sums(n_col, 0);

    u_int32_t nnz = 0;

    Cp[0] = 0;

    for (u_int32_t i = 0; i < n_row; i++)
    {
        u_int32_t head = -2;
        u_int32_t length = 0;

        u_int32_t jj_start = Ap[i];
        u_int32_t jj_end = Ap[i + 1];
        for (u_int32_t jj = jj_start; jj < jj_end; jj++)
        {
            u_int32_t j = Aj[jj];
            char v = Ax[jj];

            u_int32_t kk_start = Bp[j];
            u_int32_t kk_end = Bp[j + 1];
            for (u_int32_t kk = kk_start; kk < kk_end; kk++)
            {
                u_int32_t k = Bj[kk];

                sums[k] += v * Bx[kk];

                if (next[k] == -1)
                {
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (u_int32_t jj = 0; jj < length; jj++)
        {

            if (sums[head] != 0)
            {
                Cj[nnz] = head;
                Cx[nnz] = sums[head];
                nnz++;
            }

            u_int32_t temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
            sums[temp] = 0;
        }

        Cp[i + 1] = nnz;
    }
}

void scipy_csr_matmat_binary(const u_int32_t n_row,
                             const u_int32_t n_col,
                             const u_int32_t Ap[],
                             const u_int32_t Aj[],
                             const u_int32_t Bp[],
                             const u_int32_t Bj[],
                             u_int32_t Cp[],
                             u_int32_t Cj[])
{
    std::vector<u_int32_t> next(n_col, -1);
    std::vector<char> sums(n_col, 0);

    u_int32_t nnz = 0;

    Cp[0] = 0;

    for (u_int32_t i = 0; i < n_row; i++)
    {
        u_int32_t head = -2;
        u_int32_t length = 0;

        u_int32_t jj_start = Ap[i];
        u_int32_t jj_end = Ap[i + 1];
        for (u_int32_t jj = jj_start; jj < jj_end; jj++)
        {
            u_int32_t j = Aj[jj];
            char v = 1;

            u_int32_t kk_start = Bp[j];
            u_int32_t kk_end = Bp[j + 1];
            for (u_int32_t kk = kk_start; kk < kk_end; kk++)
            {
                u_int32_t k = Bj[kk];

                sums[k] += 1;

                if (next[k] == -1)
                {
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (u_int32_t jj = 0; jj < length; jj++)
        {

            if (sums[head] != 0)
            {
                Cj[nnz] = head;
                // Cx[nnz] = sums[head];
                nnz++;
            }

            u_int32_t temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
            sums[temp] = 0;
        }

        Cp[i + 1] = nnz;
    }
}

void scipy_csr_matmat_binary(const u_int32_t n_row,
                             const u_int32_t n_col,
                             const std::vector<u_int32_t> &Ap,
                             const std::vector<u_int32_t> &Aj,
                             const std::vector<u_int32_t> &Bp,
                             const std::vector<u_int32_t> &Bj,
                             std::vector<u_int32_t> &Cp,
                             std::vector<u_int32_t> &Cj)
{
    std::vector<u_int32_t> next(n_col, -1);
    std::vector<char> sums(n_col, 0);

    u_int32_t nnz = 0;

    Cp[0] = 0;
    size_t nbLoop = 0;
    size_t nbOp = 0;

    for (u_int32_t i = 0; i < n_row; i++)
    {
        nbLoop++;
        nbOp += 8;
        u_int32_t head = -2;
        u_int32_t length = 0;

        u_int32_t jj_start = Ap[i];
        u_int32_t jj_end = Ap[i + 1];
        for (u_int32_t jj = jj_start; jj < jj_end; jj++)
        {
            nbLoop++;
            nbOp += 6;
            u_int32_t j = Aj[jj];
            char v = 1;

            u_int32_t kk_start = Bp[j];
            u_int32_t kk_end = Bp[j + 1];
            for (u_int32_t kk = kk_start; kk < kk_end; kk++)
            {
                nbLoop++;
                nbOp += 2;
                u_int32_t k = Bj[kk];

                sums[k] += 1;

                if (next[k] == -1)
                {
                    nbOp += 3;
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (u_int32_t jj = 0; jj < length; jj++)
        {
            nbLoop++;
            nbOp += 4;
            if (sums[head] != 0)
            {
                nbOp += 2;
                Cj[nnz] = head;
                // Cx[nnz] = sums[head];
                nnz++;
            }

            u_int32_t temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
            sums[temp] = 0;
        }

        Cp[i + 1] = nnz;
    }
}

std::string scipy_tostr(const u_int32_t n_row,
                        const u_int32_t n_nz,
                        const u_int32_t Mp[],
                        const u_int32_t Mj[],
                        const char separator)
{
    if (n_row == 0)
        return "[0]\n[]";

    std::ostringstream oss;
    oss << "[";

    for (size_t i = 0; i <= n_row; ++i)
    {
        if (i)
            oss << "|";
        oss << Mp[i];
    }

    oss << "]\n[";
    for (size_t i = 0; i < n_nz; ++i)
    {
        if (i)
            oss << "|";
        oss << Mj[i];
    }
    oss << "]";

    return oss.str();
}

std::string scipy_tostr(const u_int32_t n_row,
                        const u_int32_t n_nz,
                        const std::vector<u_int32_t> &Mp,
                        const std::vector<u_int32_t> &Mj,
                        const char separator)
{
    if (n_row == 0)
        return "[0]\n[]";

    std::ostringstream oss;
    oss << "[";

    for (size_t i = 0; i <= n_row; ++i)
    {
        if (i)
            oss << "|";
        oss << Mp[i];
    }

    oss << "]\n[";
    for (size_t i = 0; i < n_nz; ++i)
    {
        if (i)
            oss << "|";
        oss << Mj[i];
    }
    oss << "]";

    return oss.str();
}

std::string scipy_tostr(const u_int32_t n_row,
                        const u_int32_t n_nz,
                        const u_int32_t Mp[],
                        const u_int32_t Mj[])
{
    return scipy_tostr(n_row, n_nz, Mp, Mj, '|');
}

std::string scipy_tostr(const u_int32_t n_row,
                        const u_int32_t n_nz,
                        const std::vector<u_int32_t> &Mp,
                        const std::vector<u_int32_t> &Mj)
{
    return scipy_tostr(n_row, n_nz, Mp, Mj, '|');
}