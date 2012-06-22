#if !defined(OUTPUT_PLAINTEXT_END_OF_PHASE_WRITER_H)
#define OUTPUT_PLAINTEXT_END_OF_PHASE_WRITER_H

#include "solving/battle_play/attack_play.h"

/* Allocate a STEndOfPhaseWriter slice.
 * @return index of allocated slice
 */
slice_index alloc_end_of_phase_writer_slice(void);

/* Try to solve in n half-moves after a defense.
 * @param si slice index
 * @param n maximum number of half moves until goal
 * @return length of solution found and written, i.e.:
 *            slack_length-2 defense has turned out to be illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type end_of_phase_writer_attack(slice_index si, stip_length_type n);

#endif
