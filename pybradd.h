#if !defined(PYBRADD_H)
#define PYBRADD_H

#include "boolean.h"
#include "pystip.h"
#include "pyslice.h"
#include "pytable.h"

/* This module provides functionality dealing with the defending side
 * in STBranchDirect stipulation slices.
 */

/* Allocate a STBranchDirectDefender defender slice.
 * @param length maximum number of half-moves of slice (+ slack)
 * @param min_length minimum number of half-moves of slice (+ slack)
 * @param next identifies next slice
 * @return index of allocated slice
 */
slice_index alloc_branch_d_defender_slice(stip_length_type length,
                                          stip_length_type min_length,
                                          slice_index next);

/* Allocate a STDirectDefenderRoot defender slice.
 * @param length maximum number of half-moves of slice (+ slack)
 * @param min_length minimum number of half-moves of slice (+ slack)
 * @param next identifies next slice
 * @param fork identifies fork slice
 * @return index of allocated slice
 */
slice_index alloc_branch_d_defender_root_slice(stip_length_type length,
                                               stip_length_type min_length,
                                               slice_index next,
                                               slice_index fork);

/* Determine whether a side has reached the goal
 * @param just_moved side that has just moved
 * @param si slice index
 * @return true iff just_moved has reached the goal
 */
boolean branch_d_defender_is_goal_reached(Side just_moved, slice_index si);

/* Determine whether the defender wins after a move by the attacker
 * @param si slice index
 * @param n (odd) number of half moves until goal
 * @param curr_max_nr_nontrivial remaining maximum number of
 *                               allowed non-trivial variations
 * @return true iff defender wins
 */
boolean branch_d_defender_does_defender_win(slice_index si,
                                            stip_length_type n,
                                            int curr_max_nr_nontrivial);

/* Solve postkey play play after the move that has just
 * been played in the current ply.
 * @param si slice index
 * @param n (odd) number of half moves until goal
 * @return true iff >=1 solution was found
 */
boolean branch_d_defender_solve_postkey_in_n(slice_index si, stip_length_type n);

/* Find solutions in next slice
 * @param si slice index
 * @return true iff >=1 solution has been found
 */
boolean branch_d_defender_solve_next(slice_index si);

/* Solve postkey play at root level.
 * @param refutations table containing the refutations (if any)
 * @param si slice index
 */
void branch_d_defender_root_solve_postkey(table refutations, slice_index si);

/* Solve at root level.
 * @param si slice index
 * @return true iff >=1 solution was found
 */
boolean branch_d_defender_root_solve(slice_index si);

/* Find refutations after a move of the attacking side at root level.
 * @param t table where to store refutations
 * @param si slice index
 * @return max_nr_refutations+1 if
 *            if the defending side is immobile (it shouldn't be here!)
 *            if the defending side has more non-trivial moves than allowed
 *            if the defending king has more flights than allowed
 *            if there is no threat in <= the maximal threat length
 *               as entered by the user
 *         number (0..max_nr_refutations) of refutations otherwise
 */
unsigned int branch_d_defender_root_find_refutations(table refutations,
                                                     slice_index si);

/* Detect starter field with the starting side if possible. 
 * @param si identifies slice
 * @param same_side_as_root does si start with the same side as root?
 * @return does the leaf decide on the starter?
 */
who_decides_on_starter
branch_d_defender_root_detect_starter(slice_index si, boolean same_side_as_root);

#endif
