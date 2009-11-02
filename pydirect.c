#include "pydirect.h"
#include "pybrad.h"
#include "pybradd.h"
#include "pybrafrk.h"
#include "pyhash.h"
#include "pyreflxg.h"
#include "pykeepmt.h"
#include "pyselfcg.h"
#include "pydirctg.h"
#include "pyselfgd.h"
#include "pyreflxg.h"
#include "pymovenb.h"
#include "pykeepmt.h"
#include "pyflight.h"
#include "pydegent.h"
#include "pythreat.h"
#include "pynontrv.h"
#include "pyleafd.h"
#include "pyint.h"
#include "trace.h"

#include <assert.h>

#define ENUMERATION_TYPENAME attack_result_type
#define ENUMERATORS \
  ENUMERATOR(attack_has_reached_deadend),       \
    ENUMERATOR(attack_refuted_full_length),    \
    ENUMERATOR(attack_solves_full_length),    \
    ENUMERATOR(attack_has_solved_next_branch)

#define ENUMERATION_MAKESTRINGS

#include "pyenum.h"

/* Try to defend after an attempted key move at root level
 * @param table table where to add refutations
 * @param si slice index
 * @return success of key move
 */
attack_result_type direct_defender_root_defend(table refutations,
                                               slice_index si)
{
  attack_result_type result = attack_has_reached_deadend;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STDirectDefenderRoot:
      result = branch_d_defender_root_defend(refutations,si);
      break;

    case STSelfAttack:
      result = self_attack_root_defend(refutations,si);
      break;

    case STReflexGuard:
      result = reflex_guard_root_defend(refutations,si);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_root_defend(refutations,si);
      break;

    case STKeepMatingGuard:
      result = keepmating_guard_root_defend(refutations,si);
      break;

    case STMaxFlightsquares:
      result = maxflight_guard_root_defend(refutations,si);
      break;

    case STMaxThreatLength:
      result = maxthreatlength_guard_root_defend(refutations,si);
      break;

    case STMaxNrNonTrivial:
      result = max_nr_nontrivial_guard_root_defend(refutations,si);
      break;

    case STRestartGuard:
      result = restart_guard_root_defend(refutations,si);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceEnumerator(attack_result_type,result,"");
  TraceFunctionResultEnd();
  return result;
}

/* Determine whether the defense just played defends against the threats.
 * @param threats table containing the threats
 * @param len_threat length of threat(s) in table threats
 * @param si slice index
 * @param n maximum number of moves until goal
 * @return true iff the defense defends against at least one of the
 *         threats
 */
boolean direct_are_threats_refuted_in_n(table threats,
                                        stip_length_type len_threat,
                                        slice_index si,
                                        stip_length_type n)
{
  boolean result = false;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",len_threat);
  TraceFunctionParam("%u",table_length(threats));
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STBranchDirect:
      result = branch_d_are_threats_refuted_in_n(threats,len_threat,si,n);
      break;

    case STDirectHashed:
      result = direct_hashed_are_threats_refuted_in_n(threats,len_threat,si,n);
      break;

    case STDirectDefense:
      result = direct_defense_are_threats_refuted_in_n(threats,len_threat,si,n);
      break;

    case STSelfDefense:
      result = self_defense_are_threats_refuted_in_n(threats,len_threat,si,n);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_are_threats_refuted_in_n(threats,len_threat,si,n);
      break;

    case STReflexGuard:
      result = reflex_guard_are_threats_refuted_in_n(threats,len_threat,si,n);
      break;

    case STKeepMatingGuard:
      result = keepmating_guard_are_threats_refuted_in_n(threats,
                                                         len_threat,
                                                         si,
                                                         n);
      break;

    case STDegenerateTree:
      result = degenerate_tree_are_threats_refuted_in_n(threats,len_threat,si,n);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Determine whether there is a solution in n half moves.
 * @param si slice index of slice being solved
 * @param n maximum number of half moves until end state has to be reached
 * @param n_min minimal number of half moves to try
 * @return length of solution found, i.e.:
 *            0 defense put defender into self-check
 *            n_min..n length of shortest solution found
 *            >n no solution found
 *         (the second case includes the situation in self
 *         stipulations where the defense just played has reached the
 *         goal (in which case n_min<slack_length_direct and we return
 *         n_min)
 */
stip_length_type direct_has_solution_in_n(slice_index si,
                                          stip_length_type n,
                                          stip_length_type n_min)
{
  stip_length_type result = n+2;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",n_min);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STBranchDirect:
      result = branch_d_has_solution_in_n(si,n,n_min);
      break;

    case STDirectHashed:
      result = direct_hashed_has_solution_in_n(si,n,n_min);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_direct_has_solution_in_n(si,n,n_min);
      break;

    case STDirectDefense:
      result = direct_defense_direct_has_solution_in_n(si,n,n_min);
      break;

    case STSelfDefense:
      result = self_defense_direct_has_solution_in_n(si,n,n_min);
      break;

    case STReflexGuard:
      result = reflex_guard_direct_has_solution_in_n(si,n,n_min);
      break;

    case STKeepMatingGuard:
      result = keepmating_guard_direct_has_solution_in_n(si,n,n_min);
      break;

    case STDegenerateTree:
      result = degenerate_tree_direct_has_solution_in_n(si,n,n_min);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Determine and write continuations after the defense just played.
 * We know that there is at least 1 continuation to the defense.
 * Only continuations of minimal length are looked for and written.
 * @param si slice index of slice being solved
 * @param n maximum number of half moves until end state has to be reached
 * @param n_min minimal number of half moves to try
 */
void direct_solve_continuations_in_n(slice_index si,
                                     stip_length_type n,
                                     stip_length_type n_min)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",n_min);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STBranchDirect:
      branch_d_solve_continuations_in_n(si,n,n_min);
      break;

    case STDirectHashed:
      direct_hashed_solve_continuations_in_n(si,n,n_min);
      break;

    case STDirectDefense:
      direct_defense_direct_solve_continuations_in_n(si,n,n_min);
      break;

    case STSelfDefense:
      self_defense_direct_solve_continuations_in_n(si,n,n_min);
      break;

    case STSelfCheckGuard:
      selfcheck_guard_direct_solve_continuations_in_n(si,n,n_min);
      break;

    case STReflexGuard:
      reflex_guard_direct_solve_continuations_in_n(si,n,n_min);
      break;

    case STKeepMatingGuard:
      keepmating_guard_direct_solve_continuations_in_n(si,n,n_min);
      break;

    case STDegenerateTree:
      degenerate_tree_direct_solve_continuations_in_n(si,n,n_min);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Determine and write the threats after the move that has just been
 * played.
 * @param threats table where to add threats
 * @param si slice index
 * @param n maximum number of half moves until goal
 * @param n_min minimal number of half moves to try
 * @return length of threats
 *         (n-slack_length_direct)%2 if the attacker has something
 *           stronger than threats (i.e. has delivered check)
 *         n+2 if there is no threat
 */
stip_length_type direct_solve_threats_in_n(table threats,
                                           slice_index si,
                                           stip_length_type n,
                                           stip_length_type n_min)
{
  stip_length_type result = n+2;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",n_min);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STBranchDirect:
      result = branch_d_solve_threats_in_n(threats,si,n,n_min);
      break;

    case STDirectHashed:
      result = direct_hashed_solve_threats_in_n(threats,si,n,n_min);
      break;

    case STDirectDefense:
      result = direct_defense_direct_solve_threats_in_n(threats,si,n,n_min);
      break;

    case STSelfDefense:
      result = self_defense_direct_solve_threats_in_n(threats,si,n,n_min);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_direct_solve_threats_in_n(threats,si,n,n_min);
      break;

    case STReflexGuard:
      result = reflex_guard_direct_solve_threats_in_n(threats,si,n,n_min);
      break;

    case STKeepMatingGuard:
      result = keepmating_guard_direct_solve_threats_in_n(threats,si,n,n_min);
      break;

    case STDegenerateTree:
      result = degenerate_tree_direct_solve_threats_in_n(threats,si,n,n_min);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Solve a slice
 * @param si slice index
 * @param n maximum number of half moves until goal
 * @param n_min minimal number of half moves to try
 * @return number of half moves effectively used
 *         n+2 if no solution was found
 *         (n-slack_length_direct)%2 if the previous move led to a
 *            dead end (e.g. self-check)
 */
stip_length_type direct_solve_in_n(slice_index si,
                                   stip_length_type n,
                                   stip_length_type n_min)
{
  stip_length_type result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",n_min);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STLeafDirect:
      result = leaf_d_solve_in_n(si,n,n_min);
      break;

    case STBranchDirect:
      result = branch_d_solve_in_n(si,n,n_min);
      break;

    case STDirectHashed:
      result = direct_hashed_solve_in_n(si,n,n_min);
      break;

    case STDirectDefense:
      result = direct_defense_solve_in_n(si,n,n_min);
      break;

    case STSelfDefense:
      result = self_defense_solve_in_n(si,n,n_min);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_solve_in_n(si,n,n_min);
      break;

    case STReflexGuard:
      result = reflex_guard_solve_in_n(si,n,n_min);
      break;

    case STDegenerateTree:
      result = direct_solve_in_n(slices[si].u.pipe.next,n,n_min);
      break;

    default:
      assert(0);
      result = n+2;
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Solve a slice - adapter for direct slices
 * @param si slice index
 * @return true iff >=1 solution was found
 */
boolean direct_solve(slice_index si)
{
  boolean result;
  stip_length_type length;
  stip_length_type min_length;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  if (slices[si].type==STLeafDirect)
  {
    length = slack_length_direct+1;
    min_length = slack_length_direct+1;
  }
  else
  {
    length = slices[si].u.pipe.u.branch.length;
    min_length = slices[si].u.pipe.u.branch.min_length;
  }

  result = direct_solve_in_n(si,length,min_length)<=length;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}


/* Solve a slice at root level
 * @param si slice index
 * @return true iff >=1 solution was found and written
 */
stip_length_type direct_root_solve_in_n(slice_index si)
{
  stip_length_type result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STLeafDirect:
      result = leaf_d_root_solve(si);
      break;

    case STDirectRoot:
      result = direct_root_root_solve(si);
      break;

    case STDirectDefenderRoot:
      result = branch_d_defender_root_solve(si);
      break;

    case STDirectDefense:
      result = direct_defense_root_solve(si);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_root_solve(si);
      break;

    case STReflexGuard:
      result = reflex_guard_root_solve(si);
      break;

    case STDirectHashed:
      result = direct_root_solve(slices[si].u.pipe.next);
      break;

    default:
      assert(0);
      result = false;
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Solve a slice at root level
 * @param si slice index
 * @return true iff >=1 solution was found
 */
boolean direct_root_solve(slice_index si)
{
  boolean result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  result = direct_root_solve_in_n(si);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Try to defend after an attempted key move at non-root level
 * @param si slice index
 * @param n maximum number of half moves until end state has to be reached
 * @return success of key move
 */
attack_result_type direct_defender_defend_in_n(slice_index si,
                                               stip_length_type n)
{
  attack_result_type result = attack_has_reached_deadend;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STBranchDirectDefender:
      result = branch_d_defender_defend_in_n(si,n);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_defend_in_n(si,n);
      break;

    case STSelfAttack:
      result = self_attack_defend_in_n(si,n);
      break;

    case STReflexGuard:
      result = reflex_guard_defend_in_n(si,n);
      break;

    case STKeepMatingGuard:
      result = keepmating_guard_defend_in_n(si,n);
      break;

    case STMaxFlightsquares:
      result = maxflight_guard_defend_in_n(si,n);
      break;

    case STMaxThreatLength:
      result = maxthreatlength_guard_defend_in_n(si,n);
      break;

    case STMaxNrNonTrivial:
      result = max_nr_nontrivial_guard_defend_in_n(si,n);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceEnumerator(attack_result_type,result,"");
  TraceFunctionResultEnd();
  return result;
}

/* Determine whether there are refutations after an attempted key move
 * at non-root level
 * @param si slice index
 * @param n maximum number of half moves until end state has to be reached
 * @param max_result how many refutations should we look for
 * @return number of refutations found (0..max_result+1)
 */
unsigned int direct_defender_can_defend_in_n(slice_index si,
                                             stip_length_type n,
                                             unsigned int max_result)
{
  boolean result = true;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParam("%u",max_result);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STDirectDefenderRoot:
    case STBranchDirectDefender:
      result = branch_d_defender_can_defend_in_n(si,n,max_result);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_can_defend_in_n(si,n,max_result);
      break;

    case STSelfAttack:
      result = self_attack_can_defend_in_n(si,n,max_result);
      break;

    case STReflexGuard:
      result = reflex_guard_can_defend_in_n(si,n,max_result);
      break;

    case STKeepMatingGuard:
      result = keepmating_guard_can_defend_in_n(si,n,max_result);
      break;

    case STMaxFlightsquares:
      result = maxflight_guard_can_defend_in_n(si,n,max_result);
      break;

    case STMaxThreatLength:
      result = maxthreatlength_guard_can_defend_in_n(si,n,max_result);
      break;

    case STMaxNrNonTrivial:
      result = max_nr_nontrivial_guard_can_defend_in_n(si,n,max_result);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Solve threats after an attacker's move
 * @param threats table where to add threats
 * @param si slice index
 * @param n maximum number of half moves until end state has to be reached
 * @return length of threats
 *         (n-slack_length_direct)%2 if the attacker has something
 *           stronger than threats (i.e. has delivered check)
 *         n+2 if there is no threat
 */
stip_length_type direct_defender_solve_threats_in_n(table threats,
                                                    slice_index si,
                                                    stip_length_type n)
{
  stip_length_type result = n+2;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STDirectDefenderRoot:
    case STBranchDirectDefender:
      result = branch_d_defender_solve_threats_in_n(threats,si,n);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_defender_solve_threats_in_n(threats,si,n);
      break;

    case STSelfAttack:
      result = self_attack_solve_threats_in_n(threats,si,n);
      break;

    case STReflexGuard:
      result = reflex_guard_defender_solve_threats_in_n(threats,si,n);
      break;

    case STKeepMatingGuard:
      result = keepmating_guard_defender_solve_threats_in_n(threats,si,n);
      break;

    case STMaxThreatLength:
      result = maxthreatlength_guard_solve_threats_in_n(threats,si,n);
      break;

    case STMaxFlightsquares:
      result = maxflight_guard_solve_threats_in_n(threats,si,n);
      break;

    case STMaxNrNonTrivial:
      result = max_nr_nontrivial_guard_solve_threats_in_n(threats,si,n);
      break;

    case STRestartGuard:
      result = restart_guard_solve_threats_in_n(threats,si,n);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}


/* Solve variations after the move that has just been played at root level
 * @param threats table containing threats
 * @param len_threat length of threats
 * @param refutations table containing refutations to move just played
 * @param si slice index
 */
void direct_defender_root_solve_variations(table threats,
                                           stip_length_type len_threat,
                                           table refutations,
                                           slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",len_threat);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STDirectDefenderRoot:
      branch_d_defender_root_solve_variations(threats,len_threat,
                                              refutations,
                                              si);
      break;

    case STSelfCheckGuard:
      selfcheck_guard_root_solve_variations(threats,len_threat,
                                            refutations,
                                            si);
      break;

    case STSelfAttack:
      self_attack_root_solve_variations(threats,len_threat,
                                        refutations,
                                        si);
      break;

    case STReflexGuard:
      reflex_guard_root_solve_variations(threats,len_threat,
                                         refutations,
                                         si);
      break;

    case STKeepMatingGuard:
      keepmating_guard_root_solve_variations(threats,len_threat,
                                             refutations,
                                             si);
      break;

    case STMaxThreatLength:
      maxthreatlength_guard_root_solve_variations(threats,len_threat,
                                                  refutations,
                                                  si);
      break;

    case STMaxFlightsquares:
      maxflight_guard_root_solve_variations(threats,len_threat,
                                            refutations,
                                            si);
      break;

    case STMaxNrNonTrivial:
      max_nr_nontrivial_guard_root_solve_variations(threats,len_threat,
                                                    refutations,
                                                    si);
      break;

    case STRestartGuard:
      restart_guard_root_solve_variations(threats,len_threat,
                                          refutations,
                                          si);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Solve variations after the move that has just been played at root level
 * @param threats table containing threats
 * @param len_threat length of threats
 * @param si slice index
 * @param n maximum length of variations to be solved
 * @return true iff >= 1 variation was found
 */
boolean direct_defender_solve_variations_in_n(table threats,
                                              stip_length_type len_threat,
                                              slice_index si,
                                              stip_length_type n)
{
  boolean result = false;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",n);
  TraceFunctionParamListEnd();

  TraceEnumerator(SliceType,slices[si].type,"\n");
  switch (slices[si].type)
  {
    case STBranchDirectDefender:
      result = branch_d_defender_solve_variations_in_n(threats,len_threat,si,n);
      break;

    case STSelfCheckGuard:
      result = selfcheck_guard_solve_variations_in_n(threats,len_threat,si,n);
      break;

    case STSelfAttack:
      result = self_attack_solve_variations_in_n(threats,len_threat,si,n);
      break;

    case STReflexGuard:
      result = reflex_guard_solve_variations_in_n(threats,len_threat,si,n);
      break;

    case STKeepMatingGuard:
      result = keepmating_guard_solve_variations_in_n(threats,len_threat,si,n);
      break;

    case STMaxThreatLength:
      result = maxthreatlength_guard_solve_variations_in_n(threats,len_threat,
                                                           si,n);
      break;

    case STMaxFlightsquares:
      result = maxflight_guard_solve_variations_in_n(threats,len_threat,si,n);
      break;

    case STMaxNrNonTrivial:
      result = max_nr_nontrivial_guard_solve_variations_in_n(threats,len_threat,
                                                             si,n);
      break;

    case STRestartGuard:
      result = restart_guard_solve_variations_in_n(threats,len_threat,si,n);
      break;

    default:
      assert(0);
      break;
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}
