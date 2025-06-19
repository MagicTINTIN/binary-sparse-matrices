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