#include "optimisations/intelligent/intercept_check_by_black.h"
#include "pyint.h"
#include "pydata.h"
#include "optimisations/intelligent/place_white_piece.h"
#include "optimisations/intelligent/place_black_piece.h"
#include "optimisations/intelligent/count_nr_of_moves.h"
#include "optimisations/intelligent/intercept_guard_by_white.h"
#include "trace.h"

#include <assert.h>
#include <stdlib.h>

/* Intercept a check to the white king with a white piece on some square
 * @param where_to_intercept where to intercept
 * @param go_on what to do after each successful interception?
 */
static void white_piece_on(square where_to_intercept, void (*go_on)(void))
{
  unsigned int i;

  TraceFunctionEntry(__func__);
  TraceSquare(where_to_intercept);
  TraceFunctionParamListEnd();

  for (i = 1; i<MaxPiece[White]; ++i)
    if (white[i].usage==piece_is_unused)
    {
      white[i].usage = piece_intercepts;
      intelligent_place_white_piece(i,where_to_intercept,go_on);
      white[i].usage = piece_is_unused;
    }

  e[where_to_intercept]= vide;
  spec[where_to_intercept]= EmptySpec;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Intercept a check to the white king with a white piece
 * @param dir_from_rider direction from rider giving check to white king
 * @param go_on what to do after each successful interception?
 */
void white_piece(int dir_from_rider, void (*go_on)(void))
{
  square where_to_intercept;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%d",dir_from_rider);
  TraceFunctionParamListEnd();

  for (where_to_intercept = king_square[White]-dir_from_rider;
       e[where_to_intercept]==vide;
       where_to_intercept -= dir_from_rider)
    if (nr_reasons_for_staying_empty[where_to_intercept]==0)
      white_piece_on(where_to_intercept,go_on);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Place a promoted black piece on the check line
 * @param intercepter_index identifies intercepting pawn
 * @param where_to_intercept what square
 * @param go_on what to do after each successful interception?
 */
static void place_black_promoteee(piece promotee_type,
                                  unsigned int intercepter_index,
                                  square where_to_intercept,
                                  void (*go_on)(void))
{
  square const intercepter_comes_from = black[intercepter_index].diagram_square;
  Flags const intercepter_flags = black[intercepter_index].flags;

  TraceFunctionEntry(__func__);
  TracePiece(promotee_type);
  TraceFunctionParam("%u",intercepter_index);
  TraceSquare(where_to_intercept);
  TraceFunctionParamListEnd();

  if (intelligent_reserve_promoting_black_pawn_moves_from_to(intercepter_comes_from,
                                                             promotee_type,
                                                             where_to_intercept))
  {
    SetPiece(promotee_type,where_to_intercept,intercepter_flags);
    (*go_on)();
    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Intercept a check to the white king with a promoted black pawn
 * @param intercepter_index identifies intercepting pawn
 * @param where_to_intercept what square
 * @param is_diagonal true iff we are intercepting a check on a diagonal line
 * @param go_on what to do after each successful interception?
 */
static void promoted_black_pawn(unsigned int intercepter_index,
                                     square where_to_intercept,
                                     boolean is_diagonal,
                                     void (*go_on)(void))
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",intercepter_index);
  TraceSquare(where_to_intercept);
  TraceFunctionParam("%u",is_diagonal);
  TraceFunctionParamListEnd();

  if (intelligent_can_promoted_black_pawn_theoretically_move_to(intercepter_index,
                                                                where_to_intercept))
  {
    piece pp;
    for (pp = -getprompiece[vide]; pp!=vide; pp = -getprompiece[-pp])
      switch (pp)
      {
        case dn:
          break;

        case tn:
          if (is_diagonal)
            place_black_promoteee(tn,intercepter_index,where_to_intercept,go_on);
          break;

        case fn:
          if (!is_diagonal)
            place_black_promoteee(fn,intercepter_index,where_to_intercept,go_on);
          break;

        case cn:
          place_black_promoteee(cn,intercepter_index,where_to_intercept,go_on);
          break;

        default:
          assert(0);
          break;
      }
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Intercept a check to the white king with an unpromoted black pawn
 * @param intercepter_index identifies intercepting pawn
 * @param where_to_intercept what square
 * @param go_on what to do after each successful interception?
 */
static void unpromoted_black_pawn(unsigned int intercepter_index,
                                       square where_to_intercept,
                                       void (*go_on)(void))
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",intercepter_index);
  TraceSquare(where_to_intercept);
  TraceFunctionParamListEnd();

  if (!black_pawn_attacks_king(where_to_intercept)
      && intelligent_reserve_black_pawn_moves_from_to_no_promotion(black[intercepter_index].diagram_square,
                                                                   where_to_intercept))
  {
    SetPiece(pn,where_to_intercept,black[intercepter_index].flags);
    (*go_on)();
    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Intercept a check to the white king with a black officer on a square
 * @param intercepter_index identifies intercepting officer
 * @param where_to_intercept what square
 * @param go_on what to do after each successful interception?
 */
static void black_officer(unsigned int intercepter_index,
                          square where_to_intercept,
                          void (*go_on)(void))
{
  piece const intercepter_type = black[intercepter_index].type;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",intercepter_index);
  TraceSquare(where_to_intercept);
  TraceFunctionParamListEnd();

  assert(!officer_guards(king_square[White],intercepter_type,where_to_intercept));
  if (intelligent_reserve_officer_moves_from_to(black[intercepter_index].diagram_square,
                                                intercepter_type,
                                                where_to_intercept))
  {
    SetPiece(intercepter_type,where_to_intercept,black[intercepter_index].flags);
    (*go_on)();
    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Intercept a check to the white king with a black piece on a square
 * @param where_to_intercept what square
 * @param is_diagonal true iff we are intercepting a check on a diagonal line
 * @param go_on what to do after each successful interception?
 */
static void black_piece_on(square where_to_intercept,
                           boolean is_diagonal,
                           void (*go_on)(void))
{
  unsigned int intercepter_index;

  TraceFunctionEntry(__func__);
  TraceSquare(where_to_intercept);
  TraceFunctionParam("%u",is_diagonal);
  TraceFunctionParamListEnd();

  for (intercepter_index = 1; intercepter_index<MaxPiece[Black]; ++intercepter_index)
    if (black[intercepter_index].usage==piece_is_unused)
    {
      black[intercepter_index].usage = piece_intercepts;

      switch (black[intercepter_index].type)
      {
        case pn:
          promoted_black_pawn(intercepter_index,where_to_intercept,
                              is_diagonal,
                              go_on);
          if (where_to_intercept>=square_a2 && where_to_intercept<=square_h7)
            unpromoted_black_pawn(intercepter_index,where_to_intercept,go_on);
          break;

        case dn:
          break;

        case tn:
          if (is_diagonal)
            black_officer(intercepter_index,where_to_intercept,go_on);
          break;

        case fn:
          if (!is_diagonal)
            black_officer(intercepter_index,where_to_intercept,go_on);
          break;

        case cn:
          black_officer(intercepter_index,where_to_intercept,go_on);
          break;

        default:
          assert(0);
          break;
      }

      black[intercepter_index].usage = piece_is_unused;
    }

  e[where_to_intercept] = vide;
  spec[where_to_intercept] = EmptySpec;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Intercept a check to the white king with a black piece
 * @param dir_from_rider direction from rider giving check to white king
 * @param go_on what to do after each successful interception?
 */
static void black_piece(int dir_to_rider, void (*go_on)(void))
{
  square const start = king_square[White]-dir_to_rider;
  boolean const is_diagonal = SquareCol(king_square[White])==SquareCol(start);
  square where_to_intercept;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%d",dir_to_rider);
  TraceFunctionParamListEnd();

  for (where_to_intercept = start;
       e[where_to_intercept]==vide;
       where_to_intercept -= dir_to_rider)
    if (nr_reasons_for_staying_empty[where_to_intercept]==0
        && *where_to_start_placing_black_pieces<=where_to_intercept)
      black_piece_on(where_to_intercept,is_diagonal,go_on);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Intercept a check to the white king
 * @param dir_from_rider direction from rider giving check to white king
 * @param go_on what to do after each successful interception?
 */
void intelligent_intercept_check_by_black(int dir_from_rider, void (*go_on)(void))
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%d",dir_from_rider);
  TraceFunctionParamListEnd();

  if (intelligent_reserve_masses(Black,1))
  {
    black_piece(dir_from_rider,go_on);
    intelligent_unreserve();
  }

  if (intelligent_reserve_masses(White,1))
  {
    white_piece(dir_from_rider,go_on);
    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
