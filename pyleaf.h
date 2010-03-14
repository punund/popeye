#if !defined(PYLEAF_H)
#define PYLEAF_H

#include "boolean.h"
#include "pystip.h"
#include "py.h"

/* This module provides functionality dealing with leaf stipulation
 * slices.
 */

/* Determine whether a side has reached the goal of a leaf slice.
 * @param just_moved side that has just moved
 * @param leaf slice index of leaf slice
 * @return whether just_moved has reached leaf's goal
 */
goal_checker_result_type leaf_is_goal_reached(Side just_moved,
                                              slice_index leaf);

/* Impose the starting side on a stipulation
 * @param si identifies branch
 * @param st address of structure that holds the state of the traversal
 */
void leaf_impose_starter(slice_index si, stip_structure_traversal *st);

/* Determine whether the mating side still has a piece that could
 * deliver the mate.
 * @return true iff the mating side has such a piece
 */
boolean is_a_mating_piece_left(Side mating_side);

/* Reset the number of obstacles that might prevent
 * generate_move_reaching_goal() from optimising by only generating
 * orthodox moves
 */
void reset_ortho_mating_moves_generation_obstacles(void);

/* Add an obstacle.
 */
void add_ortho_mating_moves_generation_obstacle(void);

/* Remove an obstacle.
 */
void remove_ortho_mating_moves_generation_obstacle(void);

/* Generate moves for side side_at_move; optimise for moves reaching a
 * specific goal. If the number of obstacles is ==0, the function
 * optimises by only generating orthodox moves.
 * @param leaf leaf slice whose goal is to be reached by generated
 *             move(s)
 * @param side_at_move side for which to generate moves
 */
void generate_move_reaching_goal(slice_index leaf, Side side_at_move);

#endif
