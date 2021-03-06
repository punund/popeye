#include "optimisations/intelligent/limit_nr_solutions_per_target.h"
#include "stipulation/stipulation.h"
#include "stipulation/pipe.h"
#include "solving/has_solution_type.h"
#include "solving/pipe.h"
#include "debugging/trace.h"

#include <stdlib.h>

/* maximum number of solutions per target positions set by user
 */
static unsigned long max_nr_solutions_per_target_position = ULONG_MAX;

/* number of solutions in the current target positions
 */
static unsigned long nr_solutions_in_current_target_position;

/* remember whether solving was affected because of the maximum number
 */
static boolean was_solving_affected;

/* Reset the number of solutions per target position
 */
void reset_nr_solutions_per_target_position(void)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  nr_solutions_in_current_target_position = 0;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Reset status whether solving the current problem was affected because the limit
 * on the number of solutions per target position was reached.
 */
void reset_was_max_nr_solutions_per_target_position_reached(void)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  was_solving_affected = false;;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Determine whether solving the current problem was affected because the limit
 * on the number of solutions per target position was reached.
 * @return true iff solving was affected
 */
boolean was_max_nr_solutions_per_target_position_reached(void)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",was_solving_affected);
  TraceFunctionResultEnd();
  return was_solving_affected;
}

/* Determine whether the maximum number of solutions per target position is
 * limited
 * @return true iff the maximum number is limited
 */
boolean is_max_nr_solutions_per_target_position_limited(void)
{
  boolean const result = max_nr_solutions_per_target_position!=ULONG_MAX;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Attempt to read the maximum number of solutions per target position
 * @param tok next1 input token
 * @return true iff the maximum number could be read from tok
 */
boolean read_max_nr_solutions_per_target_position(char const *tok)
{
  boolean result;
  char *end;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%s",tok);
  TraceFunctionParamListEnd();

  max_nr_solutions_per_target_position = strtoul(tok,&end,10);
  if (end==tok)
  {
    max_nr_solutions_per_target_position = ULONG_MAX;
    result = false;
  }
  else
    result = true;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Allocate a STIntelligentSolutionsPerTargetPosCounter slice.
 * @return index of allocated slice
 */
slice_index alloc_intelligent_nr_solutions_per_target_position_counter_slice(void)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  result = alloc_pipe(STIntelligentSolutionsPerTargetPosCounter);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Try to solve in solve_nr_remaining half-moves.
 * @param si slice index
 * @note assigns solve_result the length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played is illegal
 *            this_move_is_illegal     the move being played is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     unintended immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 *            (with n denominating solve_nr_remaining)
 */
void intelligent_nr_solutions_per_target_position_counter_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  pipe_solve_delegate(si);

  if (move_has_solved())
    ++nr_solutions_in_current_target_position;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Allocate a STIntelligentLimitNrSolutionsPerTargetPos slice.
 * @return index of allocated slice
 */
slice_index alloc_intelligent_limit_nr_solutions_per_target_position_slice(void)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  result = alloc_pipe(STIntelligentLimitNrSolutionsPerTargetPos);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Try to solve in solve_nr_remaining half-moves.
 * @param si slice index
 * @note assigns solve_result the length of solution found and written, i.e.:
 *            previous_move_is_illegal the move just played is illegal
 *            this_move_is_illegal     the move being played is illegal
 *            immobility_on_next_move  the moves just played led to an
 *                                     unintended immobility on the next move
 *            <=n+1 length of shortest solution found (n+1 only if in next
 *                                     branch)
 *            n+2 no solution found in this branch
 *            n+3 no solution found in next branch
 *            (with n denominating solve_nr_remaining)
 */
void intelligent_limit_nr_solutions_per_target_position_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  if (nr_solutions_in_current_target_position
      >=max_nr_solutions_per_target_position)
  {
    was_solving_affected = true;
    solve_result = MOVE_HAS_NOT_SOLVED_LENGTH();
  }
  else
    pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
