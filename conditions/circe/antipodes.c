#include "conditions/circe/antipodes.h"
#include "conditions/circe/circe.h"
#include "conditions/circe/capture_fork.h"
#include "stipulation/move.h"
#include "debugging/trace.h"

#include <assert.h>

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played (or being played)
 *                                     is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     unintended immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 */
stip_length_type antipodes_circe_determine_rebirth_square_solve(slice_index si,
                                                                stip_length_type n)
{
  stip_length_type result;
  move_effect_journal_index_type const base = move_effect_journal_base[nbply];
  move_effect_journal_index_type const capture = base+move_effect_journal_index_offset_capture;
  square const sq_capture = move_effect_journal[capture].u.piece_removal.from;
  unsigned int const row = sq_capture/onerow - nr_of_slack_rows_below_board;
  unsigned int const file = sq_capture%onerow - nr_of_slack_files_left_of_board;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  current_circe_rebirth_square[nbply] = sq_capture;

  if (row<nr_rows_on_board/2)
    current_circe_rebirth_square[nbply] += nr_rows_on_board/2*dir_up;
  else
    current_circe_rebirth_square[nbply] += nr_rows_on_board/2*dir_down;

  if (file<nr_files_on_board/2)
    current_circe_rebirth_square[nbply] += nr_files_on_board/2*dir_right;
  else
    current_circe_rebirth_square[nbply] += nr_files_on_board/2*dir_left;

  current_circe_rebirth_reason[nbply] = move_effect_reason_circe_rebirth;

  result = solve(slices[si].next1,n);

  current_circe_rebirth_reason[nbply] = move_effect_no_reason;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Instrument the solving machinery with Antipodes Circe
 * @param si identifies root slice of stipulation
 */
void antipodes_circe_initialise_solving(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  stip_instrument_moves(si,STCirceDetermineRebornPiece);
  stip_instrument_moves(si,STAntipodesCirceDetermineRebirthSquare);
  stip_instrument_moves(si,STCircePlaceReborn);
  stip_insert_circe_capture_forks(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
