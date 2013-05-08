#if !defined(POSITION_BOARD_H)
#define POSITION_BOARD_H

#include "utilities/bitmask.h"

/* Declarations of types and functions related to the chess board
 */

enum
{
  /* For reasons of code simplification of move generation, square a1
   * doesn't have index 0; there are some slack rows at the top and
   * bottom of the board, and some slack files at the left and right.
   */
  nr_of_slack_files_left_of_board = 8,
  nr_of_slack_rows_below_board = 8,

  nr_files_on_board = 8,
  nr_rows_on_board = 8,

  bottom_row = nr_of_slack_rows_below_board,
  top_row = bottom_row+nr_rows_on_board-1,

  left_file = nr_of_slack_files_left_of_board,
  right_file = left_file+nr_files_on_board-1,

  /* if square1-square2==onerow, then square1 is one row higher than
   * square2 */
  onerow = (nr_of_slack_files_left_of_board
            +nr_files_on_board
            +nr_of_slack_files_left_of_board),

  nr_squares_on_board = nr_files_on_board*nr_rows_on_board,

  maxsquare = ((nr_of_slack_rows_below_board
                +nr_rows_on_board
                +nr_of_slack_rows_below_board)
               *onerow),

  /* how many steps can a straight rider maximally make and still
   * remain on the board? */
  max_nr_straight_rider_steps = 7
};

/* Symbols for geometric calculations */
enum
{
  dir_left  =   -1,
  dir_right =   +1,

  dir_up    =   onerow,
  dir_down  =  -onerow
};

/* Symbols for squares */
enum
{
  square_a1 = (nr_of_slack_rows_below_board*onerow
               +nr_of_slack_files_left_of_board),
  square_b1,
  square_c1,
  square_d1,
  square_e1,
  square_f1,
  square_g1,
  square_h1,

  square_a2 = square_a1+dir_up,
  square_b2,
  square_c2,
  square_d2,
  square_e2,
  square_f2,
  square_g2,
  square_h2,

  square_a3 = square_a2+dir_up,
  square_b3,
  square_c3,
  square_d3,
  square_e3,
  square_f3,
  square_g3,
  square_h3,

  square_a4 = square_a3+dir_up,
  square_b4,
  square_c4,
  square_d4,
  square_e4,
  square_f4,
  square_g4,
  square_h4,

  square_a5 = square_a4+dir_up,
  square_b5,
  square_c5,
  square_d5,
  square_e5,
  square_f5,
  square_g5,
  square_h5,

  square_a6 = square_a5+dir_up,
  square_b6,
  square_c6,
  square_d6,
  square_e6,
  square_f6,
  square_g6,
  square_h6,

  square_a7 = square_a6+dir_up,
  square_b7,
  square_c7,
  square_d7,
  square_e7,
  square_f7,
  square_g7,
  square_h7,

  square_a8 = square_a7+dir_up,
  square_b8,
  square_c8,
  square_d8,
  square_e8,
  square_f8,
  square_g8,
  square_h8
};

enum
{
  file_a = square_a1-square_a1,
  file_b = square_b1-square_a1,
  file_c = square_c1-square_a1,
  file_d = square_d1-square_a1,
  file_e = square_e1-square_a1,
  file_f = square_f1-square_a1,
  file_g = square_g1-square_a1,
  file_h = square_h1-square_a1
};

typedef enum
{
  rot90,        /* 0 */
  rot180,       /* 1 */
  rot270,       /* 2 */
  mirra1h1,     /* 3 */
  mirra1a8,     /* 4 */
  mirra1h8,     /* 5 */
  mirra8h1      /* 6 */
} SquareTransformation;

enum
{
  initsquare = 0,      /* to initialize square-variables */
  nullsquare = 1
};

typedef int square;

typedef enum
{
  MagicSq,            /*  0 */
  WhForcedSq,         /*  1 */
  BlForcedSq,         /*  2 */
  WhConsForcedSq,     /*  3 */
  BlConsForcedSq,     /*  4 */
  NoEdgeSq,           /*  5 */
  SqColor,            /*  6 */
  WhPromSq,           /*  7 */
  BlPromSq,           /*  8 */
  WhBaseSq,           /*  9 */
  BlBaseSq,           /* 10 */
  WhPawnDoublestepSq, /* 11 */
  BlPawnDoublestepSq, /* 12 */
  CapturableByWhPawnSq, /* 13 */
  CapturableByBlPawnSq, /* 14 */
  Wormhole,           /* 15 */
  Grid,               /* 16 */

  nrSquareFlags
} SquareFlags;

extern SquareFlags zzzan[square_h8 - square_a1 + 1];
#define sq_spec         (zzzan - square_a1)

extern int         zzzao[square_h8 - square_a1 + 1];
#define sq_num          (zzzao - square_a1)

#define NoEdge(i)       TSTFLAG(sq_spec[(i)], NoEdgeSq)
#define SquareCol(i)    TSTFLAG(sq_spec[(i)], SqColor)
#define GridNum(s)      (sq_spec[(s)] >> Grid)
#define ClearGridNum(s) (sq_spec[(s)] &= ((1<<Grid)-1))

/* Calculate a square transformation
 * @param sq square to be reflected
 * @param transformation transformation to be performed
 * @return transformed value of sq
 */
square transformSquare(square sq, SquareTransformation transformation);

/* 0 terminated sequence of the effective squares of the board
 */
extern square const boardnum[65];

#endif
