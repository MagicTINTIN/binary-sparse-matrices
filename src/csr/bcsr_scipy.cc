#include "bcsr.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

void scipy_tocsc(const int n_row,
                 const int n_col,
                 const int Ap[],
                 const int Aj[],
                 //    const char Ax[],
                 int Bp[],
                 int Bi[] //,
                          //  char Bx[]
)
{
    const int nnz = Ap[n_row];

    // compute number of non-zero entries per column of A
    std::fill(Bp, Bp + n_col, 0);

    for (int n = 0; n < nnz; n++)
    {
        Bp[Aj[n]]++;
    }

    // cumsum the nnz per column to get Bp[]
    for (int col = 0, cumsum = 0; col < n_col; col++)
    {
        int temp = Bp[col];
        Bp[col] = cumsum;
        cumsum += temp;
    }
    Bp[n_col] = nnz;

    for (int row = 0; row < n_row; row++)
    {
        for (int jj = Ap[row]; jj < Ap[row + 1]; jj++)
        {
            int col = Aj[jj];
            int dest = Bp[col];

            Bi[dest] = row;
            // Bx[dest] = Ax[jj];

            Bp[col]++;
        }
    }

    for (int col = 0, last = 0; col <= n_col; col++)
    {
        int temp = Bp[col];
        Bp[col] = last;
        last = temp;
    }
}

void scipy_csr_matmat(const int n_row,
                      const int n_col,
                      const int Ap[],
                      const int Aj[],
                      const char Ax[],
                      const int Bp[],
                      const int Bj[],
                      const char Bx[],
                      int Cp[],
                      int Cj[],
                      char Cx[])
{
    std::vector<int> next(n_col, -1);
    std::vector<char> sums(n_col, 0);

    int nnz = 0;

    Cp[0] = 0;

    for (int i = 0; i < n_row; i++)
    {
        int head = -2;
        int length = 0;

        int jj_start = Ap[i];
        int jj_end = Ap[i + 1];
        for (int jj = jj_start; jj < jj_end; jj++)
        {
            int j = Aj[jj];
            char v = Ax[jj];

            int kk_start = Bp[j];
            int kk_end = Bp[j + 1];
            for (int kk = kk_start; kk < kk_end; kk++)
            {
                int k = Bj[kk];

                sums[k] += v * Bx[kk];

                if (next[k] == -1)
                {
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (int jj = 0; jj < length; jj++)
        {

            if (sums[head] != 0)
            {
                Cj[nnz] = head;
                Cx[nnz] = sums[head];
                nnz++;
            }

            int temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
            sums[temp] = 0;
        }

        Cp[i + 1] = nnz;
    }
}

void scipy_csr_matmat_binary(const int n_row,
                             const int n_col,
                             const int Ap[],
                             const int Aj[],
                             const int Bp[],
                             const int Bj[],
                             int Cp[],
                             int Cj[])
{
    std::vector<int> next(n_col, -1);
    std::vector<char> sums(n_col, 0);

    int nnz = 0;

    Cp[0] = 0;

    for (int i = 0; i < n_row; i++)
    {
        int head = -2;
        int length = 0;

        int jj_start = Ap[i];
        int jj_end = Ap[i + 1];
        for (int jj = jj_start; jj < jj_end; jj++)
        {
            int j = Aj[jj];
            char v = 1;

            int kk_start = Bp[j];
            int kk_end = Bp[j + 1];
            for (int kk = kk_start; kk < kk_end; kk++)
            {
                int k = Bj[kk];

                sums[k] += 1;

                if (next[k] == -1)
                {
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (int jj = 0; jj < length; jj++)
        {

            if (sums[head] != 0)
            {
                Cj[nnz] = head;
                // Cx[nnz] = sums[head];
                nnz++;
            }

            int temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
            sums[temp] = 0;
        }

        Cp[i + 1] = nnz;
    }
}

std::string scipy_tostr(const u_int32_t n_row,
                        const u_int32_t n_nz,
                        const int Mp[],
                        const int Mj[],
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
                        const int Mp[],
                        const int Mj[])
{
    return scipy_tostr(n_row, n_nz, Mp, Mj, '|');
}