#if !defined(CONDITIONS_MAGIC_SQUARE_H)
#define CONDITIONS_MAGIC_SQUARE_H

#include "solving/solve.h"

/* This module implements the condition Magic squares */


typedef enum
{
  magic_square_type1,
  magic_square_type2
} magic_square_type_type;

extern magic_square_type_type magic_square_type;

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
stip_length_type magic_square_side_changer_solve(slice_index si,
                                                  stip_length_type n);

/* Instrument slices with magic square side changers
 */
void stip_insert_magic_square(slice_index si);

#endif
