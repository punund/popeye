#if !defined(OPTIONS_STOPONSHORTSOLUTIONS_STOPONSHORTSOLUTIONS_H)
#define OPTIONS_STOPONSHORTSOLUTIONS_STOPONSHORTSOLUTIONS_H

#include "stipulation/stipulation.h"
#include "utilities/boolean.h"

/* This module provides functionality dealing with the filter slices
 * implement the stoponshortsolutions option and command line parameter.
 * Slices of this type make sure that solving stops after the maximum
 * time has elapsed
 */

/* Inform the stoponshortsolutions module that a short solution has
 * been found
 */
void short_solution_found(void);

/* Reset the internal state to "no short solution found" in the
 * current problem
 */
void reset_short_solution_found_in_problem(void);

/* Has a short solution been found in the current problem?
 */
boolean has_short_solution_been_found_in_problem(void);

/* Reset the internal state to "no short solution found" in the
 * current phase
 */
void reset_short_solution_found_in_phase(void);

/* Has a short solution been found in the current phase?
 */
boolean has_short_solution_been_found_in_phase(void);

/* Instrument a stipulation with STStopOnShortSolutions*Filter slices
 * @param si identifies slice where to start
 * @return true iff the option stoponshort applies
 */
boolean solving_insert_stoponshortsolutions_filters(slice_index si);

#endif
