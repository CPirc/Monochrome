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

#include <cinttypes>

#include "eval.h"
#include "position.h"
#include "types.h"

/* Piece values for material in centipawns. */
const int piecevals[2][7] = {
    { 100, 300, 300, 500, 900, 0, 0 },
    { 100, 300, 300, 500, 900, 0, 0 }
};

/* PST values in centipawns. */
const int pst[6][2][64] = {
    { // Pawns
        { // Middlegame
          0,   0,   0,   0,   0,   0,   0,   0,
         -1,  -7, -11, -35, -13,   5,   3,  -5,
          1,  1,   -6, -19,  -6,  -7,  -4,  10,
          1,  14,   8,   4,   5,   4,  10,   7,
          9,  30,  23,  31,  31,  23,  17,  11,
         21,  54,  72,  56,  77,  95,  71,  11,
        118, 121, 173, 168, 107,  82, -16,  22,
          0,   0,   0,   0,   0,   0,   0,   0
        },
        { // Endgame
          0,   0,   0,   0,   0,   0,   0,   0,
        -17, -17, -17, -17, -17, -17, -17, -17,
        -11, -11, -11, -11, -11, -11, -11, -11,
         -7,  -7,  -7,  -7,  -7,  -7,  -7,  -7,
         16,  16,  16,  16,  16,  16,  16,  16,
         55,  55,  55,  55,  55,  55,  55,  55,
         82,  82,  82,  82,  82,  82,  82,  82,
          0,   0,   0,   0,   0,   0,   0,   0
        }
    },
    { // Knights
        {
        -99, -30, -66, -64, -29, -19, -61, -81,
        -56, -31, -28,  -1,  -7, -20, -42, -11,
        -38, -16,   0,  14,   8,   3,   3, -42,
        -14,   0,   2,   3,  19,  12,  33,  -7,
        -14,  -4,  25,  33,  10,  33,  14,  43,
        -22,  18,  60,  64, 124, 143,  55,   6,
        -34,  24,  54,  74,  60, 122,   2,  29,
        -60,   0,   0,   0,   0,   0,   0,   0
        },
        {
        -99, -99, -94, -88, -88, -94, -99, -99,
        -81, -62, -49, -43, -43, -49, -62, -81,
        -46, -27, -15,  -9,  -9, -15, -27, -46,
        -22,  -3,  10,  16,  16,  10,  -3, -22,
         -7,  12,  25,  31,  31,  25,  12,  -7,
         -2,  17,  30,  36,  36,  30,  17,  -2,
         -7,  12,  25,  31,  31,  25,  12,  -7,
        -21,  -3,  10,  16,  16,  10,  -3, -21
        }
    },
    { // Bishops
        {
         -7,  12,  -8, -37, -31,  -8, -45, -67,
         15,   5,  13, -10,   1,   2,   0,  15,
          5,  12,  14,  13,  10,  -1,   3,   4,
          1,   5,  23,  32,  21,   8,  17,   4,
         -1,  16,  29,  27,  37,  27,  17,   4,
          7,  27,  20,  56,  91, 108,  53,  44,
        -24, -23,  30,  58,  65,  61,  69,  11,
          0,   0,   0,   0,   0,   0,   0,   0
        },
        {
        -27, -21, -17, -15, -15, -17, -21, -27,
        -10,  -4,   0,   2,   2,   0,  -4, -10,
          2,   8,  12,  14,  14,  12,   8,   2,
         11,  17,  21,  23,  23,  21,  17,  11,
         14,  20,  24,  26,  26,  24,  20,  14,
         13,  19,  23,  25,  25,  23,  19,  13,
          8,  14,  18,  20,  20,  18,  14,   8,
         -2,   4,   8,  10,  10,   8,   4,  -2
        }
    },
    { // Rooks
        {
         -2,  -1,   3,   1,   2,   1,   4,  -8,
        -26,  -6,   2,  -2,   2, -10,  -1, -29,
        -16,   0,   3,  -3,   8,  -1,  12,   3,
         -9,  -5,   8,  14,  18, -17,  13, -13,
         19,  33,  46,  57,  53,  39,  53,  16,
         24,  83,  54,  75, 134, 144,  85,  75,
         46,  33,  64,  62,  91,  89,  70, 104,
         84,   0,   0,  37, 124,   0,   0, 153

        },
        {
        -32, -31, -30, -29, -29, -30, -31, -32,
        -27, -25, -24, -24, -24, -24, -25, -27,
        -15, -13, -12, -12, -12, -12, -13, -15,
          1,   2,   3,   4,   4,   3,   2,   1,
         15,  17,  18,  18,  18,  18,  17,  15,
         25,  27,  28,  28,  28,  28,  27,  25,
         27,  28,  29,  30,  30,  29,  28,  27,
         16,  17,  18,  19,  19,  18,  17,  16
        }
    },
    { // Queen
        {
          1, -10, -11,   3, -15, -51, -83, -13,
         -7,   3,   2,   5,  -1, -10,  -7,  -2,
        -11,   0,  12,   2,   8,  11,   7,  -6,
         -9,   5,   7,   9,  18,  17,  26,   4,
         -6,   0,  15,  25,  32,   9,  26,  12,
        -16,  10,  13,  25,  37,  30,  15,  26,
          1,  11,  35,   0,  16,  55,  39,  57,
        -13,   6, -42,   0,  29,   0,   0, 102
        },
        {
        -61, -55, -52, -50, -50, -52, -55, -61,
        -31, -26, -22, -21, -21, -22, -26, -31,
         -8,  -3,   1,   3,   3,   1,  -3,  -8,
          9,  14,  17,  19,  19,  17,  14,   9,
         19,  24,  28,  30,  30,  28,  24,  19,
         23,  28,  32,  34,  34,  32,  28,  23,
         21,  26,  30,  31,  31,  30,  26,  21,
         12,  17,  21,  23,  23,  21,  17,  12
        }
    },
    { // King
        {
          0,   0,   0,  -9,   0,  -9,  25,   0,
         -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
         -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
         -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
         -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
         -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
         -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
         -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9
        },
        {
        -34, -30, -28, -27, -27, -28, -30, -34,
        -17, -13, -11, -10, -10, -11, -13, -17,
         -2,   2,   4,   5,   5,   4,   2,  -2,
         11,  15,  17,  18,  18,  17,  15,  11,
         22,  26,  28,  29,  29,  28,  26,  22,
         31,  34,  37,  38,  38,  37,  34,  31,
         38,  41,  44,  45,  45,  44,  41,  38,
         42,  46,  48,  50,  50,  48,  46,  42
        }
    }
};

/* Phase weights for material. */
const int phase_weights[7] = {
    0, 1, 1, 2, 4, 0, 0
};

/* Mobility weights for material. */
const int mobility_weights[7] = {
    0, 4, 4, 4, 4, 0, 0
};

/* Return material balance of a board. */
/* TODO: incremental update this. */
template<Piece p = PAWN>
int evaluate_material(const Position& pos)
{
    return popcnt(get_piece(pos, p, US)) * piecevals[OPENING][p] + evaluate_material<p+1>(pos);
}

template<>
inline int evaluate_material<QUEEN>(const Position& pos)
{
    return popcnt(get_piece(pos, QUEEN, US)) * piecevals[OPENING][QUEEN];
}

/* Return material phase of a board. */
/* 24 = opening, 0 = endgame. */
/* TODO: incremental update this. */
template<Piece p = PAWN>
int material_phase(const Position& pos)
{
    std::uint64_t pieces = get_piece(pos, p, US);

    return (popcnt(pieces) * phase_weights[p]) + (p == KING ? 0 : material_phase<p+1>(pos));
}

/* PST values for the pieces on the board. */
template<Phase ph, Piece p = PAWN>
inline int evaluate_pst(const Position& pos)
{
    int score = 0;
    uint64_t pieces = get_piece(pos, p, US);

    while (pieces) {
        score += pst[p][ph][lsb(pieces)];

        pieces &= pieces - 1;
    }

    return score + evaluate_pst<ph, p+1>(pos);
}

template<>
inline int evaluate_pst<OPENING, KING>(const Position& pos)
{
    int score = 0;
    uint64_t pieces = get_piece(pos, KING, US);

    while (pieces) {
        score += pst[KING][OPENING][lsb(pieces)];

        pieces &= pieces - 1;
    }

    return score;
}

template<>
inline int evaluate_pst<ENDGAME, KING>(const Position& pos)
{
    int score = 0;
    uint64_t pieces = get_piece(pos, KING, US);

    while (pieces) {
        score += pst[KING][ENDGAME][lsb(pieces)];

        pieces &= pieces - 1;
    }

    return score;
}

inline int king_safety(const Position& pos)
{
    std::uint64_t king_bb     = get_piece(pos, KING, US);
    Square king_sq            = lsb(king_bb);
    std::uint64_t surrounding = attacks<KING>(king_sq, (std::uint64_t)0);

    int score = 0;

    // Nearby friendly pieces
    score += 5*popcnt(surrounding & get_colour(pos, US));

    // Nearby unfriendly pieces
    //score -= 5*popcnt(surrounding & get_colour(pos, THEM));

    return score;
}

/* Evaluate piece mobility. */
template<Piece p = KNIGHT>
int evaluate_mobility(const Position& pos)
{
    int score = 0;
    uint64_t pieces = get_piece(pos, p, US);

    while (pieces) {
        score += popcnt(attacks<p>(lsb(pieces), get_occupancy(pos)))
            * mobility_weights[p];

        pieces &= pieces - 1;
    }

    return score + evaluate_mobility<p+1>(pos);
}

template<>
int evaluate_mobility<QUEEN>(const Position& pos)
{
    int score = 0;
    uint64_t pieces = get_piece(pos, QUEEN, US);

    while (pieces) {
        score += popcnt(attacks<QUEEN>(lsb(pieces), get_occupancy(pos)))
            * mobility_weights[QUEEN];

        pieces &= pieces - 1;
    }

    return score;
}

/* Return the heuristic value of a position. */
int evaluate(Position& pos)
{
    int opening = 0, endgame = 0;
    int phase = material_phase<>(pos);
    Colour side;
    int score = 0;

    for (side = US; side <= THEM; ++side) {
        opening += evaluate_material<>(pos) + evaluate_pst<OPENING>(pos);
        endgame += evaluate_material<>(pos) + evaluate_pst<ENDGAME>(pos);

        // King safety
        score += king_safety(pos);

        opening += evaluate_mobility<>(pos);
        endgame += evaluate_mobility<>(pos);

        opening = -opening;
        endgame = -endgame;
        score = -score;
        flip_position(pos);
    }

    score += ((phase * opening) + ((24 - phase) * endgame)) / 24;

    return score;
}
