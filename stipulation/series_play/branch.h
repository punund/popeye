#if !defined(STIPULATION_SERIES_PLAY_BRANCH_H)
#define STIPULATION_SERIES_PLAY_BRANCH_H

#include "py.h"
#include "pystip.h"

/* This module provides functionality dealing with STSeries*
 * stipulation slices.
 */

/* Shorten a series pipe by a half-move
 * @param pipe identifies pipe to be shortened
 */
void shorten_series_pipe(slice_index pipe);

/* Allocate a series branch.
 * @param length maximum number of half-moves of slice (+ slack)
 * @param min_length minimum number of half-moves of slice (+ slack)
 * @return index of entry slice into allocated series branch
 */
slice_index alloc_series_branch(stip_length_type length,
                                stip_length_type min_length);

/* Instrument a branch leading to a goal to be a help goal branch
 * @param si identifies entry slice of branch
 */
void stip_make_series_goal_branch(slice_index si);

/* Insert a fork to the branch leading to the goal
 * @param si identifies the entry slice of a series branch
 * @param to_goal identifies the entry slice of the branch leading to
 *                the goal
 */
void series_branch_set_goal_slice(slice_index si, slice_index to_goal);

/* Insert a fork to the next branch
 * @param si identifies the entry slice of a series branch
 * @param next identifies the entry slice of the next branch
 */
void series_branch_set_next_slice(slice_index si, slice_index next);

#endif
