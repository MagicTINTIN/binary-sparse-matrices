#include "bcsr.hh"

#include <sys/types.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

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

    // for row (i) in A's rows
    for (u_int32_t i = 0; i < n_row; i++)
    {
        u_int32_t head = -2;
        u_int32_t length = 0;

        u_int32_t jj_start = Ap[i];
        u_int32_t jj_end = Ap[i + 1];
        // for the nnz columns of row in A
        // accumulate A(i,:) * B(:,:) into sums[] (sums per B's (and C's by extension) columns)
        for (u_int32_t jj = jj_start; jj < jj_end; jj++)
        {
            u_int32_t j = Aj[jj]; // column index j in A
            char v = Ax[jj];      // A(i,j) value

            // multiply this A(i,j) by row j of B (i.e. B(j,k) for all k)
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

        // extract computed sums for row i
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
    // size_t nbLoop = 0;
    // size_t nbOp = 0;

    for (u_int32_t i = 0; i < n_row; i++)
    {
        // nbLoop++;
        // nbOp += 8;
        u_int32_t head = -2;
        u_int32_t length = 0;

        u_int32_t jj_start = Ap[i];
        u_int32_t jj_end = Ap[i + 1];
        for (u_int32_t jj = jj_start; jj < jj_end; jj++)
        {
            // nbLoop++;
            // nbOp += 6;
            u_int32_t j = Aj[jj]; // column index j in A
            char v = 1;           // A(i,j) value

            u_int32_t kk_start = Bp[j];
            u_int32_t kk_end = Bp[j + 1];
            for (u_int32_t kk = kk_start; kk < kk_end; kk++)
            {
                // nbLoop++;
                // nbOp += 2;
                u_int32_t k = Bj[kk];

                sums[k] += 1;

                if (next[k] == -1)
                {
                    // nbOp += 3;
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (u_int32_t jj = 0; jj < length; jj++)
        {
            // nbLoop++;
            // nbOp += 4;
            if (sums[head] != 0)
            {
                // nbOp += 2;
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

void my_scipy_csr_matmat_binary(const u_int32_t n_row,
                                const u_int32_t n_col,
                                const std::vector<u_int32_t> &Ap,
                                const std::vector<u_int32_t> &Aj,
                                const std::vector<u_int32_t> &Bp,
                                const std::vector<u_int32_t> &Bj,
                                std::vector<u_int32_t> &Cp,
                                std::vector<u_int32_t> &Cj)
{
    std::vector<u_int32_t> next(n_col, -1);

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
            // nbLoop++;
            // nbOp += 6;
            u_int32_t j = Aj[jj]; // column index j in A
            // char v = 1;             // A(i,j) value

            u_int32_t kk_start = Bp[j];
            u_int32_t kk_end = Bp[j + 1];
            for (u_int32_t kk = kk_start; kk < kk_end; kk++)
            {
                // nbLoop++;
                // nbOp += 2;
                u_int32_t k = Bj[kk];

                if (next[k] == -1)
                {
                    // nbOp += 3;
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (u_int32_t jj = 0; jj < length; jj++)
        {
            // nbLoop++;
            // nbOp += 4;
            // if (sums[head] != 0)
            // {
            // Cj[nnz] = head;
            //     // nbOp += 2;
            //     // Cx[nnz] = sums[head];
            // nnz++;
            // }

            Cj[nnz] = head;
            nnz++;
            u_int32_t temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
            // sums[temp] = 0;
        }

        Cp[i + 1] = nnz;
    }
}

void my_scipy_csr_matmat_binary_2(const u_int32_t n_row,
                                  const u_int32_t n_col,
                                  const std::vector<u_int32_t> &Ap,
                                  const std::vector<u_int32_t> &Aj,
                                  const std::vector<u_int32_t> &Bp,
                                  const std::vector<u_int32_t> &Bj,
                                  std::vector<u_int32_t> &Cp,
                                  std::vector<u_int32_t> &Cj)
{
    std::vector<u_int32_t> next(n_col, -1);

    u_int32_t nnz = 0;

    Cp[0] = 0;
    for (u_int32_t i = 0; i < n_row; i++)
    {
        Cp[i] = nnz;
        u_int32_t head = -2;
        u_int32_t length = 0;

        u_int32_t jj_start = Ap[i];
        u_int32_t jj_end = Ap[i + 1];
        for (u_int32_t jj = jj_start; jj < jj_end; jj++)
        {
            // nbLoop++;
            // nbOp += 6;
            u_int32_t j = Aj[jj]; // column index j in A
            // char v = 1;             // A(i,j) value

            u_int32_t kk_start = Bp[j];
            u_int32_t kk_end = Bp[j + 1];
            for (u_int32_t kk = kk_start; kk < kk_end; kk++)
            {
                // nbLoop++;
                // nbOp += 2;
                u_int32_t k = Bj[kk];

                if (next[k] == -1)
                {
                    // nbOp += 3;
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (u_int32_t jj = 0; jj < length; jj++)
        {
            // nbLoop++;
            // nbOp += 4;
            // if (sums[head] != 0)
            // {
            // Cj[nnz] = head;
            //     // nbOp += 2;
            //     // Cx[nnz] = sums[head];
            // nnz++;
            // }

            Cj[nnz] = head;
            nnz++;
            u_int32_t temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
            // sums[temp] = 0;
        }
    }
    Cp[n_row + 1] = nnz;
}

BCSR uninformed_scipy_csr_matmat_binary(const u_int32_t n_row,
                                        const u_int32_t n_col,
                                        const std::vector<u_int32_t> &Ap,
                                        const std::vector<u_int32_t> &Aj,
                                        const std::vector<u_int32_t> &Bp,
                                        const std::vector<u_int32_t> &Bj)
{
    std::vector<u_int32_t> next(n_col, -1);

    u_int32_t nnz = 0;

    std::vector<u_int32_t> index_pointers(n_row + 1);
    std::vector<u_int32_t> indices(0);
    // indices.reserve(13000);

    for (u_int32_t i = 0; i < n_row; i++)
    {
        index_pointers[i] = nnz;
        u_int32_t head = -2;
        u_int32_t length = 0;

        u_int32_t jj_start = Ap[i];
        u_int32_t jj_end = Ap[i + 1];
        for (u_int32_t jj = jj_start; jj < jj_end; jj++)
        {
            // nbLoop++;
            // nbOp += 6;
            u_int32_t j = Aj[jj]; // column index j in A
            // char v = 1;             // A(i,j) value

            u_int32_t kk_start = Bp[j];
            u_int32_t kk_end = Bp[j + 1];
            for (u_int32_t kk = kk_start; kk < kk_end; kk++)
            {
                // nbLoop++;
                // nbOp += 2;
                u_int32_t k = Bj[kk];

                if (next[k] == -1)
                {
                    // nbOp += 3;
                    next[k] = head;
                    head = k;
                    length++;
                }
            }
        }

        for (u_int32_t jj = 0; jj < length; jj++)
        {
            indices.emplace_back(head);
            // Cj[nnz] = head;
            nnz++;
            u_int32_t temp = head;
            head = next[head];

            next[temp] = -1; // clear arrays
            // sums[temp] = 0;
        }

        // Cp[i + 1] = nnz;
    }
    index_pointers[n_row] = nnz;
    bcsr_canonicalize(n_row, index_pointers, indices);
    return BCSR(n_row,n_col, index_pointers, indices);
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

std::string scipy_info(const u_int32_t width,
                        const u_int32_t height,
                        const std::vector<u_int32_t> &Mp)
{
    std::ostringstream oss;
    if (width == 0 || height == 0)
        return "<0;0>";

    std::vector<u_int32_t> nbPerLine(height);
    u_int32_t minNbPerLine(__INT32_MAX__), maxNbPerLine(0);
    for (u_int32_t r = 0; r < height; r++)
    {
        u_int32_t ones = Mp[r + 1] - Mp[r];
        nbPerLine[r] = ones;
        if (ones < minNbPerLine)
            minNbPerLine = ones;
        if (ones > maxNbPerLine)
            maxNbPerLine = ones;
    }
    std::sort(nbPerLine.begin(), nbPerLine.end());
    u_int32_t medNbPerLine = nbPerLine[height / 2];

    oss << "<" << height << ";" << width << "> ("
        << Mp[height] << " ones / "
        << (width * height)
        << ", sparsity: "
        << float(100.0 * (width * height - Mp[height]) / (width * height))
        << "%, ones per line: min="
        << minNbPerLine << ", med=" << medNbPerLine << ", max=" << maxNbPerLine
        << ")\nIndex Pointers (Rows) size: " << height + 1 << "\n"
        << "Indices (Columns) size: " << Mp[height];
    return oss.str();
}

std::string scipy_info(const u_int32_t width,
                        const u_int32_t height,
                        const u_int32_t Mp[])
{
    std::ostringstream oss;
    if (width == 0 || height == 0)
        return "<0;0>";

    std::vector<u_int32_t> nbPerLine(height);
    u_int32_t minNbPerLine(__INT32_MAX__), maxNbPerLine(0);
    for (u_int32_t r = 0; r < height; r++)
    {
        u_int32_t ones = Mp[r + 1] - Mp[r];
        nbPerLine[r] = ones;
        if (ones < minNbPerLine)
            minNbPerLine = ones;
        if (ones > maxNbPerLine)
            maxNbPerLine = ones;
    }
    std::sort(nbPerLine.begin(), nbPerLine.end());
    u_int32_t medNbPerLine = nbPerLine[height / 2];

    oss << "<" << height << ";" << width << "> ("
        << Mp[height] << " ones / "
        << (width * height)
        << ", sparsity: "
        << float(100.0 * (width * height - Mp[height]) / (width * height))
        << "%, ones per line: min="
        << minNbPerLine << ", med=" << medNbPerLine << ", max=" << maxNbPerLine
        << ")\nIndex Pointers (Rows) size: " << height + 1 << "\n"
        << "Indices (Columns) size: " << Mp[height];
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