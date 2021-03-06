#if !defined(SOLVING_POST_MOVE_ITERATION_H)
#define SOLVING_POST_MOVE_ITERATION_H

#include "solving/machinery/solve.h"
#include "solving/ply.h"

/* This module provides functionality for coordinating "post move iterations"
 * (i.e. promotions, Supercirce rebirths etc.).
 */

/* a unique number for each post move iteration (e.g. promotion,
 * rebirth square (Super Circe), king position (Republican Chess)
 */
typedef unsigned int post_move_iteration_id_type;
extern post_move_iteration_id_type post_move_iteration_id[maxply+1];

/* true iff a component has advanced its iteration; set to prevent outer
 * components from advancing theirs as well
 */
extern boolean post_move_iteration_locked[maxply+1];

/* Lock post move iterations in the current move retraction
 */
void lock_post_move_iterations(void);

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
void move_execution_post_move_iterator_solve(slice_index si);

/* Iterate if move generation uses a post-move iterating slice (e.g. for
 * MarsCirce Super)
 * @param si identifies move generator slice
 */
void move_generation_post_move_iterator_solve(slice_index si);

/* Iterate if square observation testing uses a post-move iterating slice (e.g.
 * for MarsCirce Super)
 * @param si identifies move generator slice
 */
void square_observation_post_move_iterator_solve(slice_index si);

/* Instrument the solving machinery with post move iteration slices
 * @param si identifies the root of the solving machinery
 */
void solving_insert_post_move_iteration(slice_index si);

#endif
