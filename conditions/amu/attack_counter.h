#if !defined(CONDITIONS_AMU_ATTACK_COUNTER_H)
#define CONDITIONS_AMU_ATTACK_COUNTER_H

#include "solving/machinery/solve.h"
#include "solving/ply.h"

/* This module implements the condition Traitor Chess */

extern boolean amu_attacked_exactly_once[maxply+1];

boolean amu_count_observation(slice_index si);

/* Try to solve in solve_nr_remaining half-moves.
 * @param si slice index
 * @note assigns solve_result the length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played is illegal
 *            this_move_is_illegal     the move being played is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     unintended immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 *            (with n denominating solve_nr_remaining)
 */
void amu_attack_counter_solve(slice_index si);

/* Instrument slices with move tracers
 */
void solving_insert_amu_attack_counter(slice_index si);

#endif
