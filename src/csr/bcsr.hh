#pragma once
#include <cstdint>
#ifndef BCSR_HH
#define BCSR_HH

#include <string>
#include <sys/types.h>
#include <vector>
// #include "../lil/blil.hh"
class BLIL;

std::string denseMatrixPrinter(std::vector<uint8_t> m, uint32_t height, uint32_t width);
std::string spreadsheetPrinter(std::vector<uint8_t> m, uint32_t height, uint32_t width, std::vector<std::string> linesDescription);

class BCSR {
private:
    uint32_t _width, _height;
    // uint32_t _nz_number; // equal last value of _index_pointers
    std::vector<uint32_t> _index_pointers;
    std::vector<uint32_t> _indices;

    /**
     * Insert a dense matrix in the current BCSR matrix
     * @param values[] a array-like dense matrix
     */
    void insertDn2BCSR(uint8_t values[]);

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
    BCSR(uint32_t height, uint32_t width);

    /**
     * Initialise a BCSR matrix using bcsr matrix params
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param index_pointers an array of index pointers (rows)
     * @param indices an array of indices (columns)
     */
    BCSR(uint32_t height, uint32_t width, uint32_t index_pointers[], uint32_t indices[]);

    /**
     * Initialise a BCSR matrix using bcsr matrix params
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param index_pointers an array of index pointers (rows)
     * @param indices an array of indices (columns)
     */
    BCSR(uint32_t height, uint32_t width, std::vector<uint32_t> &index_pointers, std::vector<uint32_t> &indices);

    /**
     * Initialise a BCSR matrix using a dense matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param values the dense array-like matrix values
     */
    BCSR(uint32_t height, uint32_t width, uint8_t values[]);

    /**
     * Initialise a BCSR matrix using a dense matrix
     * @param height the matrix height/rows
     * @param width the matrix width/columns
     * @param nz_number number of non zero values (optimisation to prevent too many resizes)
     */
    BCSR(uint32_t height, uint32_t width, uint32_t nz_number);

    /**
     * Initialise a BCSR matrix using a BLIL one
     * @param matrix in BLIL format
     */
    BCSR(BLIL matrix);

    // ######### Conversions to other representations #########
    /**
     * @returns a dense matrix
     */
    std::vector<uint8_t> toDenseMatrix() const;
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
    bool operator==(const BCSR &b) const;

    // ######### OR operation with matrices #########
    /**
     * @note equivalent to operation+= and operation|=
     * @returns if the matrix has changed
     */
    bool operationOr(const BCSR &b);
    /**
     * @note equivalent to operationOr method and operation+=
     * @warning different return behaviour than most of the |=, doesn't return a reference to this!
     * @returns if the matrix has changed
     */
    bool operator|=(const BCSR &b);
    /**
     * @note equivalent to operationOr method and operation|=
     * @returns a reference to the current matrix
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

    // ######### SET RESET GET VALUE #########
    /**
     * Set a value in the matrix
     * @param row of the value
     * @param col of the value
     * @param value (0 or 1) (different from 0)
     * @warning this can be a costly operation if multiply used
     */
    void set(uint32_t const row, uint32_t const col, uint8_t const value);
    /**
     * Set the value at (row,col) to 1 in the matrix
     * @param row of the value
     * @param col of the value
     * @warning this can be a costly operation if multiply used
     */
    void set(uint32_t const row, uint32_t const col);
    /**
     * Set the value at (row,col) to 0 in the matrix
     * @param row of the value
     * @param col of the value
     * @warning this can be a costly operation if multiply used
     */
    void reset(uint32_t const row, uint32_t const col);
    /**
     * Get a value in the matrix
     * @param row of the value
     * @param col of the value
     * @returns if the value at row,col is not a zero
     */
    bool get(uint32_t const row, uint32_t const col) const;

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
    std::vector<uint32_t> getRow(uint32_t row) const;

    friend class BLIL;
};

std::ostream &operator<<(std::ostream &stream, BCSR &matrix);
std::ostream &operator<<(std::ostream &stream, const BCSR &matrix);

// Scipy comparison
void scipy_csr_matmat_binary(uint32_t const n_row, uint32_t const n_col, uint32_t const Ap[], uint32_t const Aj[], uint32_t const Bp[],
                             uint32_t const Bj[], uint32_t Cp[], uint32_t Cj[]);

void scipy_csr_matmat_binary(uint32_t const n_row, uint32_t const n_col, std::vector<uint32_t> const &Ap,
                             std::vector<uint32_t> const &Aj, std::vector<uint32_t> const &Bp, std::vector<uint32_t> const &Bj,
                             std::vector<uint32_t> &Cp, std::vector<uint32_t> &Cj);

void my_scipy_csr_matmat_binary(uint32_t const n_row, uint32_t const n_col, std::vector<uint32_t> const &Ap,
                                std::vector<uint32_t> const &Aj, std::vector<uint32_t> const &Bp, std::vector<uint32_t> const &Bj,
                                std::vector<uint32_t> &Cp, std::vector<uint32_t> &Cj);

BCSR uninformed_scipy_csr_matmat_binary(uint32_t const n_row, uint32_t const n_col, std::vector<uint32_t> const &Ap,
                                        std::vector<uint32_t> const &Aj, std::vector<uint32_t> const &Bp, std::vector<uint32_t> const &Bj);

void scipy_csr_matmat(uint32_t const n_row, uint32_t const n_col, uint32_t const Ap[], uint32_t const Aj[], char const Ax[],
                      uint32_t const Bp[], uint32_t const Bj[], char const Bx[], uint32_t Cp[], uint32_t Cj[], char Cx[]);

void scipy_csr_matmat(uint32_t const n_row, uint32_t const n_col, std::vector<uint32_t> const &Ap, std::vector<uint32_t> const &Aj,
                      char const Ax[], std::vector<uint32_t> const &Bp, std::vector<uint32_t> const &Bj, char const Bx[],
                      std::vector<uint32_t> &Cp, std::vector<uint32_t> &Cj, char Cx[]);

void scipy_tocsc(uint32_t const n_row, uint32_t const n_col, uint32_t const Ap[], uint32_t const Aj[],
                 //    const char Ax[],
                 uint32_t Bp[],
                 uint32_t Bi[] //,
                               //  char Bx[]
);
// void my_scipy_tocsc(const uint32_t n_row,
//                  const uint32_t n_col,
//                  const uint32_t Ap[],
//                  const uint32_t Aj[],
//                  //    const char Ax[],
//                  uint32_t Bp[],
//                  uint32_t Bi[] //,
//                           //  char Bx[]
// );

void my_scipy_tocsc(uint32_t const n_row, uint32_t const n_col, std::vector<uint32_t> const &Ap, std::vector<uint32_t> const &Aj,
                    //    const char Ax[],
                    std::vector<uint32_t> &Bp,
                    std::vector<uint32_t> &Bi //,
                                              //  char Bx[]
);

void bcsr_canonicalize(uint32_t const n_row, uint32_t Ap[], uint32_t Aj[]);

void bcsr_canonicalize(uint32_t const n_row, std::vector<uint32_t> &Ap, std::vector<uint32_t> &Aj);

std::string scipy_tostr(uint32_t const n_row, uint32_t const n_nz, uint32_t const Mp[], uint32_t const Mj[]);

std::string scipy_tostr(uint32_t const n_row, uint32_t const n_nz, uint32_t const Mp[], uint32_t const Mj[], char const separator);

std::string scipy_tostr(uint32_t const n_row, uint32_t const n_nz, std::vector<uint32_t> const &Mp, std::vector<uint32_t> const &Mj);

std::string scipy_tostr(uint32_t const n_row, uint32_t const n_nz, std::vector<uint32_t> const &Mp, std::vector<uint32_t> const &Mj,
                        char const separator);

std::string scipy_info(uint32_t const width, uint32_t const height, std::vector<uint32_t> const &Mp);

std::string scipy_info(uint32_t const width, uint32_t const height, uint32_t const Mp[]);
#endif // BCSR_HH