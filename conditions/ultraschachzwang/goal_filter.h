#if !defined(STIPULATION_ULTRASCHACHZWANG_GOAL_FILTER_H)
#define STIPULATION_ULTRASCHACHZWANG_GOAL_FILTER_H

#include "solving/solve.h"

/* This module provides slice type STUltraschachzwangGoalFilter. This slice
 * suspends Ultraschachzwang when testing for mate.
 */

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
stip_length_type ultraschachzwang_goal_filter_solve(slice_index si, stip_length_type n);

/* Instrument a stipulation with Ultraschachzwang mate filter slices
 * @param si root of branch to be instrumented
 */
void stip_insert_ultraschachzwang_goal_filters(slice_index si);

#endif
