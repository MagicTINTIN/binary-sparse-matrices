#pragma once
#ifndef BCSR_HH
#define BCSR_HH

#include <sys/types.h>
#include <vector>
#include <string>
// #include "../lil/blil.hh"
class BLIL;

std::string denseMatrixPrinter(std::vector<u_int8_t> m, u_int32_t height, u_int32_t width);
std::string spreadsheetPrinter(std::vector<u_int8_t> m, u_int32_t height, u_int32_t width, std::vector<std::string> linesDescription);

class BCSR
{
private:
    u_int32_t _width, _height;
    // u_int32_t _nz_number; // equal last value of _index_pointers
    std::vector<u_int32_t> _index_pointers;
    std::vector<u_int32_t> _indices;

    /**
     * Insert a dense matrix in the current BCSR matrix
     * @param values[] a array-like dense matrix
     */
    void insertDn2BCSR(u_int8_t values[]);

public:
    /**
     * Initialise a <0;0> matrix
     */
    BCSR();
    /**
     * Initialise a 0-filled BCSR matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     */
    BCSR(u_int32_t height, u_int32_t width);

    /**
     * Initialise a BCSR matrix using bcsr matrix params
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param index_pointers an array of index pointers (rows)
     * @param indices an array of indices (columns)
     */
    BCSR(u_int32_t height, u_int32_t width, u_int32_t index_pointers[], u_int32_t indices[]);

    /**
     * Initialise a BCSR matrix using bcsr matrix params
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param index_pointers an array of index pointers (rows)
     * @param indices an array of indices (columns)
     */
    BCSR(u_int32_t height, u_int32_t width, std::vector<u_int32_t> &index_pointers, std::vector<u_int32_t> &indices);

    /**
     * Initialise a BCSR matrix using a dense matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param values the dense array-like matrix values
     */
    BCSR(u_int32_t height, u_int32_t width, u_int8_t values[]);

    /**
     * Initialise a BCSR matrix using a dense matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param nz_number number of non zero values (optimisation to prevent too many resizes)
     */
    BCSR(u_int32_t height, u_int32_t width, u_int32_t nz_number);

    /**
     * Initialise a BCSR matrix using a BLIL one
     * @param matrix in BLIL format
     */
    BCSR(BLIL matrix);

    // ######### Conversions to other representations #########
    /**
     * @returns a dense matrix
     */
    std::vector<u_int8_t> toDenseMatrix() const;
    /**
     * Convert the BCSR matrix to a printable string
     */
    std::string toString() const;
    /**
     * Convert the BCSR matrix to a printable condensed string
     */
    std::string toCondensedString() const;
    /**
     * Convert the BCSR matrix to a printable condensed string
     */
    std::string toCondensedString(char const separator) const;
    /**
     * Get information about the matrix
     */
    std::string info() const;
    /**
     * Convert the BCSR matrix in dense form to a printable string
     */
    std::string toDnString() const;
    /**
     * Convert the BCSR matrix in dense form to a printable string with indices
     */
    std::string toSpreadsheet() const;
    /**
     * Convert the BCSR matrix in dense form to a printable string with indices
     * @param linesDescription a vector of string with a string per line
     */
    std::string toSpreadsheet(std::vector<std::string> linesDescription) const;
    /**
     * Convert the BCSR matrix to a BLIL one
     */
    BLIL toBLIL() const;

    /**
     * check ordering of the indices
     * CHECK: remove ?
     * @note useless in prod
     */
    bool checkOrder() const;
    /**
     * check ordering of the indices
     * @param verbose to print what are the representation issues
     * CHECK: remove ?
     * @note useless in prod
     */
    bool checkOrder(bool verbose) const;

    /**
     * Check if 2 matrices are the same
     */
    bool operator==(const BCSR &b);

    // ######### OR operation with matrices #########
    /**
     * @note equivalent to operation+= and operation|=
     */
    void operationOr(const BCSR &b);
    /**
     * @note equivalent to operationOr method and operation+=
     */
    BCSR &operator|=(const BCSR &b);
    /**
     * @note equivalent to operationOr method and operation|=
     */
    BCSR &operator+=(const BCSR &b);
    BCSR operator|(const BCSR &b) const;
    BCSR operator+(const BCSR &b) const;

    // ######### AND operation with matrices #########
    /**
     * @note equivalent to hadamard multiplication
     */
    void operationAnd(const BCSR &b);
    /**
     * @note equivalent to hadamard multiplication
     */
    BCSR operator&(const BCSR &b) const;
    /**
     * @note equivalent to hadamard multiplication
     */
    BCSR &operator&=(const BCSR &b);

    // ######### MULTIPLY operation with matrice matrice #########
    /**
     * @note not fully implemented, only for square matrices
     */
    BCSR operationTimesMatrix(const BCSR &b) const;
    /**
     * @note not fully implemented, only for square matrices
     */
    BCSR operator*(const BCSR &b) const;
    /**
     * @note not fully implemented, only for square matrices
     */
    BCSR &operator*=(const BCSR &b);

    // ######### TRANSPOSITION operation #########
    /**
     * Will transpose the matrix
     * @warning it modifies the current matrix!
     * @note equivalent to transform it as a CSC
     */
    BCSR &selfTranspose();
    /**
     * @note equivalent to transform it as a CSC
     * @returns the transposed matrix
     */
    BCSR transpose() const;

    // ######### SET RESET VALUE #########
    /**
     * Set a value in the matrix
     * @param row of the value
     * @param col of the value
     * @param value (0 or 1) (different from 0)
     * @warning this can be a costly operation
     */
    void set(const u_int32_t row, const u_int32_t col, const u_int8_t value);
    /**
     * Set the value at (row,col) to 1 in the matrix
     * @param row of the value
     * @param col of the value
     * @warning this can be a costly operation
     */
    void set(const u_int32_t row, const u_int32_t col);
    /**
     * Set the value at (row,col) to 0 in the matrix
     * @param row of the value
     * @param col of the value
     * @warning this can be a costly operation
     */
    void reset(const u_int32_t row, const u_int32_t col);

    /**
     * Add an empty row and column to the end of the matrix
     */
    void addDimension();

    /**
     * Add an empty column to the end of the matrix
     */
    void addColumn();

    /**
     * Add an empty row to the end of the matrix
     */
    void addRow();

    // Not needed for the moment
    // ~BCSR();

    /**
     * Retrieve non zeros columns of a row
     * @param row
     * @warning no row check
     */
    std::vector<u_int32_t> getRow(u_int32_t row) const;

    friend class BLIL;
};

std::ostream &operator<<(std::ostream &stream, BCSR &matrix);
std::ostream &operator<<(std::ostream &stream, const BCSR &matrix);

// Scipy comparison
void scipy_csr_matmat_binary(const u_int32_t n_row,
                             const u_int32_t n_col,
                             const u_int32_t Ap[],
                             const u_int32_t Aj[],
                             const u_int32_t Bp[],
                             const u_int32_t Bj[],
                             u_int32_t Cp[],
                             u_int32_t Cj[]);

void scipy_csr_matmat_binary(const u_int32_t n_row,
                             const u_int32_t n_col,
                             const std::vector<u_int32_t> &Ap,
                             const std::vector<u_int32_t> &Aj,
                             const std::vector<u_int32_t> &Bp,
                             const std::vector<u_int32_t> &Bj,
                             std::vector<u_int32_t> &Cp,
                             std::vector<u_int32_t> &Cj);

void my_scipy_csr_matmat_binary(const u_int32_t n_row,
                                const u_int32_t n_col,
                                const std::vector<u_int32_t> &Ap,
                                const std::vector<u_int32_t> &Aj,
                                const std::vector<u_int32_t> &Bp,
                                const std::vector<u_int32_t> &Bj,
                                std::vector<u_int32_t> &Cp,
                                std::vector<u_int32_t> &Cj);

BCSR uninformed_scipy_csr_matmat_binary(const u_int32_t n_row,
                                        const u_int32_t n_col,
                                        const std::vector<u_int32_t> &Ap,
                                        const std::vector<u_int32_t> &Aj,
                                        const std::vector<u_int32_t> &Bp,
                                        const std::vector<u_int32_t> &Bj);

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
                      char Cx[]);

void scipy_csr_matmat(const u_int32_t n_row,
                      const u_int32_t n_col,
                      const std::vector<u_int32_t> &Ap,
                      const std::vector<u_int32_t> &Aj,
                      const char Ax[],
                      const std::vector<u_int32_t> &Bp,
                      const std::vector<u_int32_t> &Bj,
                      const char Bx[],
                      std::vector<u_int32_t> &Cp,
                      std::vector<u_int32_t> &Cj,
                      char Cx[]);

void scipy_tocsc(const u_int32_t n_row,
                 const u_int32_t n_col,
                 const u_int32_t Ap[],
                 const u_int32_t Aj[],
                 //    const char Ax[],
                 u_int32_t Bp[],
                 u_int32_t Bi[] //,
                                //  char Bx[]
);
// void my_scipy_tocsc(const u_int32_t n_row,
//                  const u_int32_t n_col,
//                  const u_int32_t Ap[],
//                  const u_int32_t Aj[],
//                  //    const char Ax[],
//                  u_int32_t Bp[],
//                  u_int32_t Bi[] //,
//                           //  char Bx[]
// );

void my_scipy_tocsc(const u_int32_t n_row,
                    const u_int32_t n_col,
                    const std::vector<u_int32_t> &Ap,
                    const std::vector<u_int32_t> &Aj,
                    //    const char Ax[],
                    std::vector<u_int32_t> &Bp,
                    std::vector<u_int32_t> &Bi //,
                                               //  char Bx[]
);

void bcsr_canonicalize(const u_int32_t n_row,
                        u_int32_t Ap[],
                        u_int32_t Aj[]);

void bcsr_canonicalize(const u_int32_t n_row,
                        std::vector<u_int32_t> &Ap,
                        std::vector<u_int32_t> &Aj);

std::string scipy_tostr(const u_int32_t n_row,
                        const u_int32_t n_nz,
                        const u_int32_t Mp[],
                        const u_int32_t Mj[]);

std::string scipy_tostr(const u_int32_t n_row,
                        const u_int32_t n_nz,
                        const u_int32_t Mp[],
                        const u_int32_t Mj[],
                        const char separator);

std::string scipy_tostr(const u_int32_t n_row,
                        const u_int32_t n_nz,
                        const std::vector<u_int32_t> &Mp,
                        const std::vector<u_int32_t> &Mj);

std::string scipy_tostr(const u_int32_t n_row,
                        const u_int32_t n_nz,
                        const std::vector<u_int32_t> &Mp,
                        const std::vector<u_int32_t> &Mj,
                        const char separator);

std::string scipy_info(const u_int32_t width,
                       const u_int32_t height,
                       const std::vector<u_int32_t> &Mp);

std::string scipy_info(const u_int32_t width,
                       const u_int32_t height,
                       const u_int32_t Mp[]);
#endif // BCSR_HH