#if !defined(OUTPUT_PLAINTEXT_TREE_REFUTATION_WRITER_H)
#define OUTPUT_PLAINTEXT_TREE_REFUTATION_WRITER_H

#include "solving/solve.h"

/* Allocate a STRefutationsIntroWriter slice.
 * @return index of allocated slice
 */
slice_index alloc_refutations_intro_writer_slice(void);

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
stip_length_type refutations_intro_writer_solve(slice_index si,
                                                 stip_length_type n);

/* Allocate a STRefutationWriter slice.
 * @return index of allocated slice
 */
slice_index alloc_refutation_writer_slice(void);

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
stip_length_type refutation_writer_solve(slice_index si, stip_length_type n);

#endif
