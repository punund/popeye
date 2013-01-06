#if !defined(STIPULATION_MOVE_INVERTER_H)
#define STIPULATION_MOVE_INVERTER_H

#include "stipulation/structure_traversal.h"
#include "solving/solve.h"

/* This module provides functionality dealing with move inverter slices
 */

/* Allocate a STMoveInverter slice.
 * @return index of allocated slice
 */
slice_index alloc_move_inverter_slice(void);

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
stip_length_type move_inverter_solve(slice_index si, stip_length_type n);

/* Detect starter field with the starting side if possible.
 * @param si identifies slice being traversed
 * @param st status of traversal
 */
void move_inverter_detect_starter(slice_index si, stip_structure_traversal *st);

#endif
