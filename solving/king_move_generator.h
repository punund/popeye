#if !defined(SOLVING_KING_MOVE_GENERATOR_H)
#define SOLVING_KING_MOVE_GENERATOR_H

#include "solving/solve.h"

/* This module provides functionality dealing with the attacking side
 * in STKingMoveGenerator stipulation slices.
 */

/* Allocate a STKingMoveGenerator slice.
 * @return index of allocated slice
 */
slice_index alloc_king_move_generator_slice(void);

/* Generate moves for the king (if any) of a side
 * @param side side for which to generate king moves
 */
void generate_king_moves(Side side);

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
stip_length_type king_move_generator_solve(slice_index si, stip_length_type n);

#endif
