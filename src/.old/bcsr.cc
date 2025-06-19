//// SET & RESET replaced by dichotomic version

// void BCSR::set(const u_int32_t row, const u_int32_t col)
// {
//     if (col >= _width || row >= _height)
//     {
//         fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
//         exit(EXIT_FAILURE);
//     }

//     bool inserted = false;
//     // for each row after the insertion, propagate the information
//     for (size_t r = row + 1; r <= _height; r++) // < _index_pointers.size()
//     {
//         if (!inserted)
//         {
//             for (u_int32_t i = _index_pointers[r - 1]; i < _index_pointers[r]; i++)
//             {
//                 if (col < _indices[i])
//                 {
//                     _indices.insert(_indices.begin() + i, col);
//                     // _nz_number++;
//                     inserted = true;
//                     break;
//                 }
//                 // if the value was already 1, then there is no change
//                 else if (col == _indices[i])
//                     return;
//             }
//             if (!inserted)
//             {
//                 _indices.insert(_indices.begin() + _index_pointers[r], col);
//                 // _nz_number++;
//                 inserted = true;
//             }
//         }
//         _index_pointers[r] += 1;
//     }
// }


// void BCSR::reset(const u_int32_t row, const u_int32_t col)
// {
//     if (col >= _width || row >= _height)
//     {
//         fprintf(stderr, "Error: position does not match in set method, accessing M<%d;%d>(%d,%d)\n", _height, _width, row, col);
//         exit(EXIT_FAILURE);
//     }

//     bool removed = false;
//     // for each row after the removal, propagate the information
//     for (size_t r = row + 1; r <= _height; r++)
//     {
//         if (!removed)
//         {
//             for (u_int32_t i = _index_pointers[r - 1]; i < _index_pointers[r]; i++)
//             {
//                 if (col == _indices[i])
//                 {
//                     _indices.erase(_indices.begin() + i);
//                     // _nz_number--;
//                     removed = true;
//                     // the column has been deleted, there is nothing more to check, if we wanted to, think to add i--;
//                     break;
//                 }
//             }
//             if (!removed)
//             {
//                 // if nothing has been removed on the affected row, then there is nothing to update
//                 return;
//             }
//         }
//         _index_pointers[r] -= 1;
//     }
// }


// REPLACED BY CONVERSION TO LIL

// void BCSR::operationOr(const BCSR &b)
// {
//     // *this = *this | b;
//     if (b._width != _width || b._height != _height)
//     {
//         fprintf(stderr, "Error: dimensions does not match in operationOr a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
//         exit(EXIT_FAILURE);
//     }

//    // for each row after an insertion, propagate the information with the carry
//        u_int32_t carry = 0;
//        for (size_t r = 1; r <= _height; r++) //< _index_pointers.size()
//        {
//            // if the line contains a non zero value.
//            if (b._index_pointers[r] - b._index_pointers[r - 1] > 0)
//            {
//                u_int32_t col_index = _index_pointers[r - 1];
//                u_int32_t b_index_pointer = b._index_pointers[r - 1];
//                // for each column check if there is an insertion to do before it
//                for (; col_index < _index_pointers[r] + carry; col_index++)
//                {
//                    // add every non zero indice (column) before this column
//                    for (; b._indices[b_index_pointer] <= _indices[col_index] && b_index_pointer < b._index_pointers[r]; b_index_pointer++)
//                    {
//                        // if the value was already 1, then there is no change for this column
//                        if (b._indices[b_index_pointer] < _indices[col_index])
//                        {
//                            _indices.insert(_indices.begin() + col_index, b._indices[b_index_pointer]);
//                            carry++;
//                            col_index++;
//                        }
//                    }
//                }
//                // for every other non-zero in the line that has not been added
//                for (; b_index_pointer < b._index_pointers[r]; b_index_pointer++)
//                {
//                    _indices.insert(_indices.begin() + col_index, b._indices[b_index_pointer]);
//                    carry++;
//                }
//            }
//            _index_pointers[r] += carry;
//        } 
// }

// void BCSR::operationAnd(const BCSR &b)
// {
//     if (b._width != _width || b._height != _height)
//     {
//         fprintf(stderr, "Error: dimensions does not match in operationAnd a<%d;%d> != b<%d;%d>\n", _height, _width, b._height, b._width);
//         exit(EXIT_FAILURE);
//     }

//     // for each row after an removal, propagate the information with the carry
//     u_int32_t carry = 0;
//     for (size_t r = 1; r <= _height; r++)
//     {
//         if (_index_pointers[r] - _index_pointers[r - 1] == 0)
//         {
//         }
//         // if the line pf b contains a non zero value.
//         else if (b._index_pointers[r] - b._index_pointers[r - 1] > 0)
//         {
//             u_int32_t col_index = _index_pointers[r - 1];
//             u_int32_t b_index_pointer = b._index_pointers[r - 1];
//             // for each column of b
//             for (; b_index_pointer < b._index_pointers[r]; b_index_pointer++)
//             {
//                 // cehck if the current matrix has non-zero that has the same columns
//                 for (; _indices[col_index] <= b._indices[b_index_pointer] && col_index < _index_pointers[r] - carry; col_index++)
//                 {
//                     // if the value of a previous column is a 1, we need to delete it
//                     if (_indices[col_index] < b._indices[b_index_pointer])
//                     {
//                         _indices.erase(_indices.begin() + col_index);
//                         col_index--;
//                         carry++;
//                     }
//                     // else we just continue looking in the line for the other columns of b
//                 }
//             }
//             // for every other 1 -> they are now 0
//             _indices.erase(_indices.begin() + col_index, _indices.begin() + _index_pointers[r] - carry);
//             carry += _index_pointers[r] - carry - col_index;
//             // for (size_t idx_ptr = col_index; idx_ptr < _index_pointers[r] - carry;)
//             // {
//             //     _indices.erase(_indices.begin() + idx_ptr);
//             //     carry++;
//             // }
//         }
//         // if we still have non-zeros left on this line, then remove all of them
//         else // if (_index_pointers[r] - _index_pointers[r - 1] > 0) non necessary now
//         {
//             for (size_t idx_ptr = _index_pointers[r - 1]; idx_ptr < _index_pointers[r] - carry; idx_ptr++)
//             {
//                 _indices.erase(_indices.begin() + idx_ptr);
//                 idx_ptr--;
//                 carry++;
//             }
//         }
//         _index_pointers[r] -= carry;
//     }
// }