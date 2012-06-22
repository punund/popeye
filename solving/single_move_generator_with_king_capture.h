#if !defined(SOLVING_SINGLE_MOVE_GENERATOR_WITH_KING_CAPTURE_H)
#define SOLVING_SINGLE_MOVE_GENERATOR_WITH_KING_CAPTURE_H

#include "solving/battle_play/attack_play.h"

/* This module provides functionality dealing with the attacking side
 * in STSingleMoveGeneratorWithKingCapture stipulation slices.
 */

/* Allocate a STSingleMoveGeneratorWithKingCapture slice.
 * @return index of allocated slice
 */
slice_index alloc_single_move_generator_with_king_capture_slice(void);

/* Initialise the next move generation
 * @param sq_departure departure square of move to be generated
 * @param sq_arrival arrival square of move to be generated
 * @param sq_capture capture square of move to be generated
 */
void single_move_generator_with_king_capture_init_next(square sq_departure,
                                                  square sq_arrival,
                                                  square sq_capture);

/* Try to solve in n half-moves after a defense.
 * @param si slice index
 * @param n maximum number of half moves until end state has to be reached
 * @return length of solution found and written, i.e.:
 *            slack_length-2 defense has turned out to be illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type single_move_generator_with_king_capture_attack(slice_index si,
                                                                stip_length_type n);

#endif
