#if !defined(STIPULATION_BATTLE_PLAY_BRANCH_H)
#define STIPULATION_BATTLE_PLAY_BRANCH_H

#include "pystip.h"

/* This module provides functionality dealing with battle play
 * branches
 */

/* Allocate a branch that represents battle play
 * @param length maximum number of half-moves of slice (+ slack)
 * @param min_length minimum number of half-moves of slice (+ slack)
 * @return index of entry slice of allocated branch
 */
slice_index alloc_battle_branch(stip_length_type length,
                                stip_length_type min_length);

/* Calculate the minimum number of moves to solve for depending of the
 * maximum number and the minimum and maximum length of a slice
 * @param si identifies battle play branch slice
 * @param n maximum number of moves
 * @return minimum number of moves
 */
stip_length_type battle_branch_calc_n_min(slice_index si, stip_length_type n);

#endif
