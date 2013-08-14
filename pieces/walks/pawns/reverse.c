#include "pieces/walks/pawns/reverse.h"
#include "pieces/walks/pawns/pawns.h"
#include "pieces/walks/pawns/pawn.h"
#include "pieces/walks/pawns/en_passant.h"
#include "solving/move_generator.h"
#include "solving/find_square_observer_tracking_back_from_target.h"
#include "debugging/trace.h"
#include "pydata.h"

#include <assert.h>

/* Generate moves for a reverse pawn
 */
void reverse_pawn_generate_moves(void)
{
  unsigned int const no_capture_length = pawn_get_no_capture_length(advers(trait[nbply]),
                                                                    curr_generation->departure);

  if (no_capture_length>0)
  {
    int const dir_backward = trait[nbply]==White ? dir_down : dir_up;

    pawns_generate_capture_move(dir_backward+dir_right);
    pawns_generate_capture_move(dir_backward+dir_left);
    pawns_generate_nocapture_moves(dir_backward,no_capture_length);
  }
}

boolean reversepawn_check(evalfunction_t *evaluate)
{
  square const sq_target = move_generation_stack[current_move[nbply]-1].capture;
  SquareFlags const capturable = trait[nbply]==White ? CapturableByBlPawnSq : CapturableByWhPawnSq;

  if (TSTFLAG(sq_spec[sq_target],capturable) || observing_walk[nbply]==Orphan || observing_walk[nbply]>=Hunter0)
  {
    numvec const dir_backward = trait[nbply]==White ? dir_down : dir_up;
    numvec const dir_backward_right = dir_backward+dir_right;
    numvec const dir_backward_left = dir_backward+dir_left;

    if (pawn_test_check(sq_target-dir_backward_right,sq_target,evaluate))
      return true;
    else if (pawn_test_check(sq_target-dir_backward_left,sq_target,evaluate))
      return true;
    else if (en_passant_test_check(dir_backward_right,&pawn_test_check,evaluate))
      return true;
    else if (en_passant_test_check(dir_backward_left,&pawn_test_check,evaluate))
      return true;
  }

  return false;
}
