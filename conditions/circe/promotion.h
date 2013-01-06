#if !defined(CONDITIONS_CIRCE_PROMOTION_H)
#define CONDITIONS_CIRCE_PROMOTION_H

#include "solving/solve.h"

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
stip_length_type circe_promoter_solve(slice_index si, stip_length_type n);

/* Instrument a stipulation for promotion of Circe reborns (only possible in
 * some Circe variations)
 * @param si identifies root slice of stipulation
 */
void stip_insert_circe_promoters(slice_index si);

#endif
