# Binary Sparse Matrices
> Sparse matrices library specialized in binary values.

Transposition and matrix-matrix multiplication algorithms are adapted from the [**SciPy**](https://github.com/scipy/scipy/) library for Python.

## Classes
- BCSR (binary compressed sparse row) class implementation
- BLIL (binary LInked List) class implementation

Operations imply to have canonical CSR, with **sorted** and **unique** indices (columns)

## Features implemented
### CSR
- [x] conversion from/to dense matrix/terminal spreadsheet/string
- [x] conversion from/to LIL matrix
- [x] matrix-matrix multiplication
- [ ] matrix-vector multiplication
- [x] matrix addition/Or
- [x] matrix-matrix Hadamard multiplication/And
- [x] transposition
- [x] constructor with CSR parameters
- [x] set/reset value
- [x] growing matrices
- [ ] non-zero growing
- [x] multiple stats and information on matrices
- [x] equal operator
- [ ] get specific row (non-zero columns) and read-access to full matrix structure
### LIL
- [x] conversion from/to CSR matrix/terminal spreadsheet/string
- [x] conversion from/to dense matrix
- [x] transposition
- [x] matrix addition/Or
- [x] matrix-matrix Hadamard multiplication/And
- [x] matrix-matrix multiplication
- [ ] matrix-vector multiplication
- [x] constructor with LIL parameters
- [x] set/reset value
- [x] growing matrices
- [x] multiple stats and information on matrices
- [x] equal operator
- [x] get specific row (non-zero columns) and read-access to full matrix structure

---

### Tools

You can also find matrix generators in python (and C++ for better performances for big matrices).