# Binary Sparse Matrices
> Sparse matrices implementations specialized with binary values (0/1)

## Classes
- BCSR (binary compressed sparse row) class implementation
- BLIL (binary LInked List) class implementation

Operations imply to have canonical CSR, with **sorted** and **unique** indices (columns)

## Features implemented
### CSR
- [x] conversion from/to dense matrix
- [x] conversion from/to LIL matrix
- [x] matrix-matrix multiplication
- [ ] matrix-vector multiplication
- [x] matrix addition/or
- [x] matrix-matrix Hadamard multiplication/and
- [x] transposition
- [x] constructor with CSR parameters
- [x] set/reset value
### LIL
- [x] conversion from/to CSR matrix
- [x] conversion from/to dense matrix
- [ ] transposition
- [ ] matrix addition/or
- [ ] matrix-matrix Hadamard multiplication/and
- [ ] matrix-matrix multiplication
- [ ] matrix-vector multiplication
- [x] constructor with LIL parameters
- [x] set/reset value