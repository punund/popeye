#include "stipulation/goals/prerequisite_optimiser.h"
#include "stipulation/stipulation.h"
#include "stipulation/pipe.h"
#include "pydata.h"
#include "stipulation/has_solution_type.h"
#include "stipulation/branch.h"
#include "stipulation/goals/prerequisite_guards.h"
#include "solving/avoid_unsolvable.h"
#include "debugging/trace.h"

#include <assert.h>

/* Allocate a STPrerequisiteOptimiser defender slice.
 * @return index of allocated slice
 */
slice_index alloc_goal_prerequisite_optimiser_slice(void)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  result = alloc_pipe(STPrerequisiteOptimiser);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

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
stip_length_type
goal_prerequisite_optimiser_solve(slice_index si, stip_length_type n)
{
  stip_length_type result;
  slice_index const next = slices[si].next1;
  stip_length_type const save_max_unsolvable = max_unsolvable;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  assert(max_unsolvable<=slack_length);

  if (goal_preprequisites_met[nbply]==0)
  {
    max_unsolvable = slack_length+1;
    TraceValue("->%u\n",max_unsolvable);
  }

  result = solve(next,n);

  max_unsolvable = save_max_unsolvable;
  TraceValue("->%u\n",max_unsolvable);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}
