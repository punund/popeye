#include "stipulation/goals/enpassant/reached_tester.h"
#include "stipulation/pipe.h"
#include "stipulation/goals/reached_tester.h"
#include "stipulation/boolean/true.h"
#include "debugging/trace.h"
#include "debugging/assert.h"

/* This module provides functionality dealing with slices that detect
 * whether a enpassant goal has just been reached
 */

/* Allocate a system of slices that tests whether enpassant has been reached
 * @return index of entry slice
 */
slice_index alloc_goal_enpassant_reached_tester_system(void)
{
  slice_index result;
  slice_index enpassant_tester;
  Goal const goal = { goal_ep, initsquare };

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  enpassant_tester = alloc_pipe(STGoalEnpassantReachedTester);
  pipe_link(enpassant_tester,alloc_true_slice());
  result = alloc_goal_reached_tester_slice(goal,enpassant_tester);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}
