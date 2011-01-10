#include "pythreat.h"
#include "pydata.h"
#include "pypipe.h"
#include "stipulation/branch.h"
#include "stipulation/battle_play/branch.h"
#include "stipulation/battle_play/attack_play.h"
#include "stipulation/battle_play/check_detector.h"
#include "trace.h"

#include <assert.h>
#include <stdlib.h>

static stip_length_type max_len_threat;

/* Reset the max threats setting to off
 */
void reset_max_threat_length(void)
{
  max_len_threat = no_stip_length;
}

/* Read the requested max threat length setting from a text token
 * entered by the user
 * @param textToken text token from which to read
 * @return true iff max threat setting was successfully read
 */
boolean read_max_threat_length(const char *textToken)
{
  boolean result;
  char *end;
  unsigned long const requested_max_threat_length = strtoul(textToken,&end,10);

  if (textToken!=end && requested_max_threat_length<=UINT_MAX)
  {
    max_len_threat = (stip_length_type)requested_max_threat_length;
    result = true;
  }
  else
    result = false;

  return result;
}

/* Retrieve the current max threat length setting
 * @return current max threat length setting
 *         no_stip_length if max threats option is not active
 */
stip_length_type get_max_threat_length(void)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",max_len_threat);
  TraceFunctionResultEnd();
  return max_len_threat;
}

/* **************** Private helpers ***************
 */

/* Determine whether the threat after the attacker's move just played
 * is too long respective to user input.
 * @param si slice index
 * @param n maximum number of half moves until goal
 * @return true iff threat is too long
 */
static boolean is_threat_too_long(slice_index si,
                                  stip_length_type n,
                                  stip_length_type n_max)
{
  boolean result;
  stip_length_type const n_max_unsolvable = slack_length_battle;
  slice_index const
      to_attacker = slices[si].u.maxthreatlength_guard.to_attacker;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  result = (n_max
            <attack_has_solution_in_n(to_attacker,n_max-1,n_max_unsolvable-1));

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* **************** Initialisation ***************
 */

/* Allocate a STMaxThreatLength slice
 * @param length maximum number of half moves until goal
 * @param to_attacker identifies slice leading to attacker
 */
static slice_index alloc_maxthreatlength_guard(stip_length_type length,
                                               slice_index to_attacker)
{
  slice_index result;
  stip_length_type const parity = (length-slack_length_battle)%2;
  stip_length_type const min_length = slack_length_battle+parity;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",length);
  TraceFunctionParam("%u",to_attacker);
  TraceFunctionParamListEnd();

  result = alloc_branch(STMaxThreatLength,length,min_length);
  slices[result].u.maxthreatlength_guard.to_attacker = to_attacker;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}


/* Try to defend after an attacking move
 * When invoked with some n, the function assumes that the key doesn't
 * solve in less than n half moves.
 * @param si slice index
 * @param n maximum number of half moves until end state has to be reached
 * @param n_max_unsolvable maximum number of half-moves that we
 *                         know have no solution
 * @return <=n solved  - return value is maximum number of moves
 *                       (incl. defense) needed
 *         n+2 refuted - acceptable number of refutations found
 *         n+4 refuted - >acceptable number of refutations found
 */
stip_length_type
maxthreatlength_guard_defend_in_n(slice_index si,
                                  stip_length_type n,
                                  stip_length_type n_max_unsolvable)
{
  slice_index const next = slices[si].u.pipe.next;
  stip_length_type result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",n_max_unsolvable);
  TraceFunctionParamListEnd();

  /* we already know whether the attack move has delivered check, so
     let's deal with that first */
  if (attack_gives_check[nbply])
    result = defense_defend_in_n(next,n,n_max_unsolvable);
  else if (max_len_threat==0)
    result = n+4;
  else
  {
    stip_length_type const n_max = 2*(max_len_threat-1)+slack_length_battle+2;
    if (n>=n_max)
    {
      if (is_threat_too_long(si,n,n_max))
        result = n+4;
      else
        result = defense_defend_in_n(next,n,n_max_unsolvable);
    }
    else
      /* remainder of play is too short for max_len_threat to apply */
      result = defense_defend_in_n(next,n,n_max_unsolvable);
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Determine whether there are defenses after an attacking move
 * @param si slice index
 * @param n maximum number of half moves until end state has to be reached
 * @param n_max_unsolvable maximum number of half-moves that we
 *                         know have no solution
 * @return <=n solved  - return value is maximum number of moves
 *                       (incl. defense) needed
 *         n+2 refuted - <=acceptable number of refutations found
 *         n+4 refuted - >acceptable number of refutations found
 */
stip_length_type
maxthreatlength_guard_can_defend_in_n(slice_index si,
                                      stip_length_type n,
                                      stip_length_type n_max_unsolvable)
{
  slice_index const next = slices[si].u.pipe.next;
  unsigned int result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",n_max_unsolvable);
  TraceFunctionParamListEnd();

  /* determine whether the attack move has delivered check is
     expensive, so let's try to avoid it */
  if (max_len_threat==0)
  {
    if (echecc(nbply,slices[si].starter))
      result = defense_defend_in_n(next,n,n_max_unsolvable);
    else
      result = n+4;
  }
  else
  {
    stip_length_type const n_max = 2*(max_len_threat-1)+slack_length_battle+2;
    if (n>=n_max)
    {
      if (echecc(nbply,slices[si].starter))
        result = defense_can_defend_in_n(next,n,n_max_unsolvable);
      else if (is_threat_too_long(si,n,n_max))
        result = n+4;
      else
        result = defense_can_defend_in_n(next,n,n_max_unsolvable);
    }
    else
      /* remainder of play is too short for max_len_threat to apply */
      result = defense_can_defend_in_n(next,n,n_max_unsolvable);
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}


/* **************** Stipulation instrumentation ***************
 */

/* Insert a STMaxThreatLength slice before each defender slice
 * @param si identifies defender slice
 * @param st address of struct representing the traversal
 */
static void maxthreatlength_guard_inserter(slice_index si,
                                           stip_structure_traversal *st)
{
  stip_length_type const length = slices[si].u.branch.length;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  if (max_len_threat==0
      || length>=2*(max_len_threat-1)+slack_length_battle+2)
  {
    boolean * const inserted = st->param;
    slice_index const
        to_attacker = branch_find_slice(STDefenseMoveLegalityChecked,si);
    slice_index const prototype = alloc_maxthreatlength_guard(length,to_attacker);
    battle_branch_insert_slices(si,&prototype,1);
    *inserted = true;
  }

  stip_traverse_structure_children(si,st);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static structure_traversers_visitors maxthreatlength_guards_inserters[] =
{
  { STReadyForDefense, &maxthreatlength_guard_inserter }
};

enum
{
  nr_maxthreatlength_guards_inserters =
  (sizeof maxthreatlength_guards_inserters
   / sizeof maxthreatlength_guards_inserters[0])
};

/* Instrument stipulation with STMaxThreatLength slices
 * @param si identifies slice where to start
 */
boolean stip_insert_maxthreatlength_guards(slice_index si)
{
  boolean result;
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  TraceStipulation(si);

  stip_structure_traversal_init(&st,&result);
  stip_structure_traversal_override(&st,
                                    maxthreatlength_guards_inserters,
                                    nr_maxthreatlength_guards_inserters);
  stip_traverse_structure(si,&st);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}
