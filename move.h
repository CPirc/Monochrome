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

#ifndef MOVE_H
#define MOVE_H

#include "types.h"

/*
 * The move encoding scheme:
 * From Squares - 64 => 6 bits
 * To Squares   - 64 => 6 bits
 * Move Types   - 7  => 3 bits
 * Prom Types   - 4  => 3 bits (Will be storing the piece type and queen is 4)
 */

#define TO_SQ_SHIFT     (6)
#define MOVE_TYPE_SHIFT (12)
#define PROM_TYPE_SHIFT (15)

#define FROM_SQ_MASK   (0x3f)
#define TO_SQ_MASK     (0xfc0)
#define MOVE_TYPE_MASK (0x7000)
#define PROM_TYPE_MASK (0x38000)

/* The various types of moves */
enum MoveType : unsigned int {
    NORMAL,
    CASTLE       = 1 << MOVE_TYPE_SHIFT,
    CAPTURE      = 2 << MOVE_TYPE_SHIFT,
    ENPASSANT    = 3 << MOVE_TYPE_SHIFT,
    PROMOTION    = 4 << MOVE_TYPE_SHIFT,
    DOUBLE_PUSH  = 5 << MOVE_TYPE_SHIFT,
    PROM_CAPTURE = 6 << MOVE_TYPE_SHIFT
};

/* The various promotion types of a pawn */
enum PromotionType : unsigned int {
    NONE,
    TO_KNIGHT = KNIGHT << PROM_TYPE_SHIFT,
    TO_BISHOP = BISHOP << PROM_TYPE_SHIFT,
    TO_ROOK   = ROOK   << PROM_TYPE_SHIFT,
    TO_QUEEN  = QUEEN  << PROM_TYPE_SHIFT
};

typedef unsigned int Move;

static inline Square from_square(const Move move)
{
    return Square(move & FROM_SQ_MASK);
}

static inline Square to_square(const Move move)
{
    return Square((move & TO_SQ_MASK) >> TO_SQ_SHIFT);
}

static inline MoveType move_type(const Move move)
{
    return MoveType((move & MOVE_TYPE_MASK) >> MOVE_TYPE_SHIFT);
}

static inline PromotionType promotion_type(const Move move)
{
    return PromotionType((move & PROM_TYPE_MASK) >> PROM_TYPE_SHIFT);
}

static inline Move get_move(Square from, Square to, MoveType move_type, PromotionType prom_type = NONE)
{
    return Move(from | (to << TO_SQ_SHIFT) | (move_type << MOVE_TYPE_SHIFT) | (prom_type << PROM_TYPE_SHIFT));
}

#endif