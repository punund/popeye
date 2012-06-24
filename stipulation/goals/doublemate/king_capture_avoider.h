#if !defined(STIPULATION_GOAL_DOUBLEMATE_KING_CAPTURE_AVOIDER_H)
#define STIPULATION_GOAL_DOUBLEMATE_KING_CAPTURE_AVOIDER_H

#include "solving/battle_play/attack_play.h"
#include "solving/battle_play/defense_play.h"

/* This module provides functionality to avoid king capture in immobility
 * testing for double (and counter) mate goals.
 * According to current knowledge, this can only happen if the king to be
 * mated is neutral.
 */

/* Instrument a stipulation
 * @param si identifies root slice of stipulation
 */
void stip_insert_king_capture_avoiders(slice_index si);

/* Try to solve in n half-moves after a defense.
 * @param si slice index
 * @param n maximum number of half moves until goal
 * @return length of solution found and written, i.e.:
 *            slack_length-2 defense has turned out to be illegal
 *            <=n length of shortest solution found
 *            n+2 no solution found
 */
stip_length_type king_capture_avoider_attack(slice_index si,
                                             stip_length_type n);

/* Try to defend after an attacking move
 * When invoked with some n, the function assumes that the key doesn't
 * solve in less than n half moves.
 * @param si slice index
 * @param n maximum number of half moves until end state has to be reached
 * @return <slack_length - no legal defense found
 *         <=n solved  - <=acceptable number of refutations found
 *                       return value is maximum number of moves
 *                       (incl. defense) needed
 *         n+2 refuted - >acceptable number of refutations found
 */
stip_length_type king_capture_avoider_defend(slice_index si,
                                             stip_length_type n);

#endif
