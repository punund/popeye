#if !defined(STIPULATION_BATTLE_PLAY_ATTACK_PLAY_H)
#define STIPULATION_BATTLE_PLAY_ATTACK_PLAY_H

/* Interface for dynamically dispatching attack operations to slices
 * depending on the slice type
 */

#include "pyslice.h"

/* Determine whether there is a solution in n half moves, by trying
 * n_min, n_min+2 ... n half-moves.
 * @param si slice index of slice being solved
 * @param n maximum number of half moves until end state has to be reached
 * @param n_min minimal number of half moves to try
 * @param n_max_unsolvable maximum number of half-moves that we
 *                         know have no solution
 * @return length of solution found, i.e.:
 *            n_min-2 defense has turned out to be illegal
 *            n_min..n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type attack_has_solution_in_n(slice_index si,
                                          stip_length_type n,
                                          stip_length_type n_min,
                                          stip_length_type n_max_unsolvable);

/* Determine whether a slice has a solution - adapter for direct slices
 * @param si slice index
 * @return whether there is a solution and (to some extent) why not
 */
has_solution_type attack_has_solution(slice_index si);

/* Solve a slice, by trying n_min, n_min+2 ... n half-moves.
 * @param si slice index
 * @param n maximum number of half moves until goal
 * @param n_min minimum number of half-moves of interesting variations
 * @param n_max_unsolvable maximum number of half-moves that we
 *                         know have no solution
 * @return length of solution found and written, i.e.:
 *            n_min-2 defense has turned out to be illegal
 *            n_min..n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type attack_solve_in_n(slice_index si,
                                   stip_length_type n,
                                   stip_length_type n_min,
                                   stip_length_type n_max_unsolvable);

/* Solve a slice
 * @param si slice index
 * @return whether there is a solution and (to some extent) why not
 */
has_solution_type attack_solve(slice_index si);

#endif
