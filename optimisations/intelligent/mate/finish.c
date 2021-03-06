#include "optimisations/intelligent/mate/finish.h"
#include "stipulation/stipulation.h"
#include "solving/has_solution_type.h"
#include "solving/machinery/solve.h"
#include "solving/castling.h"
#include "solving/conditional_pipe.h"
#include "optimisations/intelligent/intelligent.h"
#include "optimisations/intelligent/count_nr_of_moves.h"
#include "optimisations/intelligent/place_black_piece.h"
#include "optimisations/intelligent/place_white_king.h"
#include "optimisations/orthodox_square_observation.h"
#include "debugging/trace.h"
#include "pieces/pieces.h"

#include "debugging/assert.h"

/* Place any black piece on some square
 * @param placed_on where to place any black piece
 */
static void place_any_black_piece_on(square placed_on)
{
  unsigned int placed_index;

  TraceFunctionEntry(__func__);
  TraceSquare(placed_on);
  TraceFunctionParamListEnd();

  if (intelligent_reserve_masses(Black,1,piece_intercepts))
  {
    for (placed_index = 1; placed_index<MaxPiece[Black]; ++placed_index)
      if (black[placed_index].usage==piece_is_unused)
      {
        black[placed_index].usage = piece_intercepts;
        intelligent_place_black_piece(placed_index,
                                      placed_on,
                                      &intelligent_mate_test_target_position);
        black[placed_index].usage = piece_is_unused;
      }

    empty_square(placed_on);

    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Is there a redundant white piece in the curren position?
 * @return true iff there is a redundant piece
 */
static boolean exists_redundant_white_piece(void)
{
  boolean result = false;
  square const *bnp;
  castling_rights_type const save_castling_flag = being_solved.castling_rights;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  /* we temporarily disable Black castling for two reasons:
   * 1. we are solving from the target position here where king or rook may be
   *    at different positions than in the diagram; attempting to generate
   *    (let alone) execute castling moves would cause problems in this case
   * 2. Black is in check - we don't need to test for it again
   */
  CLRCASTLINGFLAGMASK(Black,k_cancastle);

  /* check for redundant white pieces */
  for (bnp = boardnum; !result && *bnp!=initsquare; bnp++)
  {
    square const sq = *bnp;
    if (TSTFLAG(being_solved.spec[sq],White) && !TSTFLAG(being_solved.spec[sq],Royal))
    {
      PieceIdType const id = GetPieceId(being_solved.spec[sq]);
      piece_usage const usage = white[PieceId2index[id]].usage;
      TraceValue("%u",PieceId2index[id]);
      TraceSquare(*bnp);
      TraceEnumerator(piece_usage,usage,"\n");
      if (usage!=piece_intercepts_check_from_guard && usage!=piece_gives_check)
      {
        piece_walk_type const p = get_walk_of_piece_on_square(sq);
        Flags const sp = being_solved.spec[sq];
        empty_square(sq);
        result = conditional_pipe_solve_delegate(current_start_slice)==previous_move_has_solved;
        occupy_square(sq,p,sp);
      }
    }
  }

  being_solved.castling_rights = save_castling_flag;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Find a black king flight that may have been created while placing pieces
 * after guarding and blocking
 * @return the first flight square found; initsquare if none was found
 */
static square find_king_flight(void)
{
  square result = initsquare;
  vec_index_type i;
  Flags const king_spec = being_solved.spec[being_solved.king_square[Black]];

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  empty_square(being_solved.king_square[Black]);

  for (i = vec_queen_start; i<=vec_queen_end; ++i)
  {
    square const flight = being_solved.king_square[Black]+vec[i];

    if (!is_square_blocked(flight)
        && !TSTFLAG(being_solved.spec[flight],Black)
        && !is_square_observed_ortho(White,flight))
    {
      result = flight;
      break;
    }
  }

  occupy_square(being_solved.king_square[Black],King,king_spec);

  TraceFunctionExit(__func__);
  TraceSquare(result);
  TraceFunctionResultEnd();
  return result;
}

/* fix the white king on its diagram square
 */
static void fix_white_king_on_diagram_square(void)
{
  square const king_diagram_square = white[index_of_king].diagram_square;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  if (is_square_empty(king_diagram_square)
      && nr_reasons_for_staying_empty[king_diagram_square]==0
      && intelligent_reserve_masses(White,1,piece_is_king))
  {
    white[index_of_king].usage = piece_is_king;
    intelligent_place_white_king(king_diagram_square,
                                 &intelligent_mate_test_target_position);
    white[index_of_king].usage = piece_is_unused;

    intelligent_unreserve();
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Test the position created by the taken operations; if the position is a
 * solvable target position: solve it; otherwise: improve it
 */
void intelligent_mate_test_target_position(void)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  /*assert(!echecc(White));*/

  {
    square const flight = find_king_flight();
    if (flight==initsquare)
    {
      /* Nail white king to diagram square if no white move remains; we can't do
       * this with the other unused white or black pieces because they might be
       * captured in the solution */
      if (white[index_of_king].usage==piece_is_unused
          && white[index_of_king].diagram_square!=square_e1
          && intelligent_get_nr_remaining_moves(White)==0)
        fix_white_king_on_diagram_square();
      else if (!exists_redundant_white_piece())
        solve_target_position();
    }
    else if (is_square_empty(flight))
      place_any_black_piece_on(flight);
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
