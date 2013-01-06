#if !defined(CONDITIONS_TAKE_MAKE_CIRCE_H)
#define CONDITIONS_TAKE_MAKE_CIRCE_H

/* This module implements regular Take&Make Circe (aka Anti-Take&Make)
 */

#include "solving/solve.h"

extern numecoup take_make_circe_current_rebirth_square_index[maxply+1];

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played (or being played)
 *                                     is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     uninted immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 */
stip_length_type take_make_circe_collect_rebirth_squares_solve(slice_index si,
                                                               stip_length_type n);

/* Try to solve in n half-moves.
 * @param si slice index
 * @param n maximum number of half moves
 * @return length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played (or being played)
 *                                     is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     uninted immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 */
stip_length_type take_make_circe_determine_rebirth_squares_solve(slice_index si,
                                                                 stip_length_type n);

/* Instrument a stipulation for Circe
 * @param si identifies root slice of stipulation
 */
void stip_insert_take_make_circe(slice_index si);

#endif
