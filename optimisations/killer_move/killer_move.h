#if !defined(OPTIMISATION_KILLER_MOVE_H)
#define OPTIMISATION_KILLER_MOVE_H

#include "pystip.h"

/* Reset the enabled state of the optimisation
 */
void reset_killer_move_optimisation(void);

/* Disable the optimisation for defense moves by one side
 * @param side side for which to disable the optimisation
 */
void disable_killer_move_optimisation(Side);

/* Instrument stipulation with killer move slices
 * @param si identifies slice where to start
 */
void stip_optimise_with_killer_moves(slice_index si);

#endif
