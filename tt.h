/*
MIT License

Copyright (c) 2017 CPirc

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef TT_H
#define TT_H

#include "types.h"
#include "misc.h"

std::uint64_t piece_sq_keys[2][6][64];
std::uint64_t castle_keys[16];
std::uint32_t side_keys[2];

struct TranspositionTable {
    std::uint64_t data;
    std::uint64_t hash_key;
};

inline void initialize_keys()
{
    int i, j, k;
    for (i = 0; i < 2; ++i) {
        side_keys[i] = get_rand64();
        for (j = 0; j < 16; ++j) {
            castle_keys[j] = get_rand64();
        }
        for (j = 0; j < 6; ++j) {
            for (k = 0; k < 64; ++k) {
                piece_sq_keys[i][j][k] = get_rand64();
            }
        }
    }
}

#endif