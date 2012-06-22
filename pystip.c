#include "pystip.h"
#include "stipulation/fork.h"
#include "stipulation/pipe.h"
#include "stipulation/move_inverter.h"
#include "stipulation/branch.h"
#include "stipulation/setplay_fork.h"
#include "stipulation/binary.h"
#include "stipulation/goals/reached_tester.h"
#include "stipulation/boolean/or.h"
#include "stipulation/boolean/and.h"
#include "stipulation/boolean/or.h"
#include "stipulation/boolean/not.h"
#include "stipulation/boolean/true.h"
#include "stipulation/battle_play/branch.h"
#include "stipulation/battle_play/attack_adapter.h"
#include "stipulation/battle_play/defense_adapter.h"
#include "stipulation/move_played.h"
#include "stipulation/goals/immobile/reached_tester.h"
#include "stipulation/help_play/adapter.h"
#include "stipulation/proxy.h"
#include "optimisations/intelligent/mate/filter.h"
#include "optimisations/intelligent/stalemate/filter.h"
#include "debugging/trace.h"

#include <assert.h>


Slice slices[max_nr_slices];


/* Create a slice
 * @param type which type
 * @return index of created slice
 */
slice_index create_slice(slice_type type)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceEnumerator(slice_type,type,"");
  TraceFunctionParamListEnd();

  result = alloc_slice();

  slices[result].type = type;
  slices[result].starter = no_side;
  slices[result].prev = no_slice;
  slices[result].next1 = no_slice;
  slices[result].next2 = no_slice;
  slices[result].tester = no_slice;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Allocate a slice as copy of an existing slice
 * @param index of original slice
 * @return index of allocated slice
 */
slice_index copy_slice(slice_index original)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",original);
  TraceFunctionParamListEnd();

  result = create_slice(slices[original].type);

  slices[result] = slices[original];
  slice_set_predecessor(result,no_slice);

  TraceEnumerator(Side,slices[original].starter,"");
  TraceEnumerator(Side,slices[result].starter,"\n");

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Deallocate slices reachable from a slice
 * @param si slice where to start deallocating
 */
void dealloc_slices(slice_index si)
{
  slice_index i;
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  TraceStipulation(si);

  stip_structure_traversal_init(&st,0);
  stip_traverse_structure(si,&st);

  for (i = 0; i!=max_nr_slices; ++i)
    if (st.traversed[i]==slice_traversed)
      dealloc_slice(i);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Make a slice the predecessor of a slice
 * @param slice identifies the slice
 * @param pred identifies the slice to be made the predecessor of slice
 */
void slice_set_predecessor(slice_index slice, slice_index pred)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",slice);
  TraceFunctionParam("%u",pred);
  TraceFunctionParamListEnd();

  slices[slice].prev = pred;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void move_to_root(slice_index si, stip_structure_traversal *st)
{
  spin_off_state_type * const state = st->param;
  slice_index const save_next = slices[si].next1;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  stip_traverse_structure_children_pipe(si,st);

  link_to_branch(si,state->spun_off[save_next]);
  state->spun_off[si] = si;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static structure_traversers_visitor root_slice_inserters[] =
{
  { STAttackAdapter,  &attack_adapter_make_root  },
  { STDefenseAdapter, &defense_adapter_make_root },
  { STHelpAdapter,    &help_adapter_make_root    },
  { STAnd,            &binary_make_root          },
  { STOr,             &binary_make_root          }
};

enum
{
  nr_root_slice_inserters = (sizeof root_slice_inserters
                             / sizeof root_slice_inserters[0])
};

/* Initialise a spin_off_state_type object
 */
static void spin_off_state_init(spin_off_state_type *state)
{
  slice_index i;
  for (i = 0; i!=max_nr_slices; ++i)
    state->spun_off[i] = no_slice;
}

/* Wrap the slices representing the initial moves of the solution with
 * slices of appropriately equipped slice types
 * @param si identifies slice where to start
 */
void stip_insert_root_slices(slice_index si)
{
  stip_structure_traversal st;
  spin_off_state_type state;
  slice_index const next = slices[si].next1;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  TraceStipulation(si);
  assert(slices[si].type==STProxy);

  spin_off_state_init(&state);
  stip_structure_traversal_init(&st,&state);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_pipe,
                                                 &move_to_root);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_branch,
                                                 &move_to_root);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_fork,
                                                 &move_to_root);
  stip_structure_traversal_override(&st,
                                    root_slice_inserters,
                                    nr_root_slice_inserters);
  stip_traverse_structure(next,&st);

  pipe_link(si,state.spun_off[next]);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void link_to_intro(slice_index si, stip_structure_traversal *st)
{
  stip_traverse_structure_children(si,st);

  /* make sure that the entry slices into the intro have a correct .prev value */
  link_to_branch(si,slices[si].next1);
}

void hack_fork_make_intro(slice_index fork, stip_structure_traversal *st)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",fork);
  TraceFunctionParamListEnd();

  stip_traverse_structure_children_pipe(fork,st);

  st->level = structure_traversal_level_nested;
  stip_traverse_structure_conditional_pipe_tester(fork,st);
  st->level = structure_traversal_level_top;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static structure_traversers_visitor intro_slice_inserters[] =
{
  { STAttackAdapter,     &attack_adapter_make_intro   },
  { STDefenseAdapter,    &defense_adapter_make_intro  },
  { STHelpAdapter,       &help_adapter_make_intro     },
  { STTemporaryHackFork, &hack_fork_make_intro        }
};

enum
{
  nr_intro_slice_inserters = (sizeof intro_slice_inserters
                              / sizeof intro_slice_inserters[0])
};

/* Wrap the slices representing the initial moves of nested slices
 * @param si identifies slice where to start
 */
void stip_insert_intro_slices(slice_index si)
{
  spin_off_state_type state;
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  TraceStipulation(si);
  assert(slices[si].type==STProxy);

  spin_off_state_init(&state);
  stip_structure_traversal_init(&st,&state);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_pipe,
                                                 &link_to_intro);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_branch,
                                                 &link_to_intro);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_fork,
                                                 &link_to_intro);
  stip_structure_traversal_override(&st,
                                    intro_slice_inserters,
                                    nr_intro_slice_inserters);
  stip_traverse_structure(si,&st);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

typedef struct
{
  Goal unique_goal;
  boolean is_unique;
} find_unique_goal_state;

static void find_unique_goal_goal(slice_index si,
                                  stip_structure_traversal *st)
{
  find_unique_goal_state * const state = st->param;
  goal_type const goal = slices[si].u.goal_handler.goal.type;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  if (state->unique_goal.type==no_goal)
    state->unique_goal.type = goal;
  else if (state->is_unique && state->unique_goal.type!=goal)
    state->is_unique = false;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Determine whether the current stipulation has a unique goal, and
 * return it.
 * @param si root of subtree where to look for unique goal
 * @return no_slice if goal is not unique; index of a slice with the
 * unique goal otherwise
 */
Goal find_unique_goal(slice_index si)
{
  stip_structure_traversal st;
  find_unique_goal_state result = { { no_goal, initsquare }, true };

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  stip_structure_traversal_init(&st,&result);
  stip_structure_traversal_override_single(&st,
                                           STGoalReachedTester,
                                           &find_unique_goal_goal);
  stip_traverse_structure(si,&st);

  if (!result.is_unique)
    result.unique_goal.type = no_goal;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result.unique_goal.type);
  TraceFunctionResultEnd();
  return result.unique_goal;
}

static void copy_and_remember(slice_index si, stip_deep_copies_type *copies)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  assert((*copies)[si]==no_slice);
  (*copies)[si] = copy_slice(si);
  slices[(*copies)[si]].starter = no_side;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void deep_copy_leaf(slice_index si, stip_structure_traversal *st)
{
  stip_deep_copies_type * const copies = st->param;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  copy_and_remember(si,copies);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void deep_copy_pipe(slice_index si, stip_structure_traversal *st)
{
  stip_deep_copies_type * const copies = st->param;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  copy_and_remember(si,copies);

  stip_traverse_structure_children_pipe(si,st);

  if (slices[si].next1!=no_slice)
    link_to_branch((*copies)[si],(*copies)[slices[si].next1]);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void deep_copy_fork(slice_index si, stip_structure_traversal *st)
{
  stip_deep_copies_type * const copies = st->param;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  copy_and_remember(si,copies);

  stip_traverse_structure_children(si,st);

  if (slices[si].next2!=no_slice)
    slices[(*copies)[si]].next2 = (*copies)[slices[si].next2];

  if (slices[si].next1!=no_slice)
    link_to_branch((*copies)[si],(*copies)[slices[si].next1]);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void deep_copy_binary(slice_index si, stip_structure_traversal *st)
{
  stip_deep_copies_type * const copies = st->param;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  copy_and_remember(si,copies);

  stip_traverse_structure_children(si,st);

  if (slices[si].next1!=no_slice)
    slices[(*copies)[si]].next1 = (*copies)[slices[si].next1];

  if (slices[si].next2!=no_slice)
    slices[(*copies)[si]].next2 = (*copies)[slices[si].next2];

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void deep_copy_zigzag_jump(slice_index si, stip_structure_traversal *st)
{
  stip_deep_copies_type * const copies = st->param;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  deep_copy_binary(si,st);

  if (slices[si].u.if_then_else.condition!=no_slice)
    slices[(*copies)[si]].u.if_then_else.condition = (*copies)[slices[si].u.if_then_else.condition];

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Initialise a structure traversal for a deep copy operation
 * @param st address of the structure to be initialised
 * @param st_parent parent traversal (0 if there is none)
 * @param copies address of an array mapping indices of originals
 *        to indices of copies
 * @note initialises all elements of *copies to no_slice
 * @note after this initialisation, *st can be used for a deep copy operation;
 *       or st can be further modified for some special copy operation
 */
void init_deep_copy(stip_structure_traversal *st,
                    stip_structure_traversal *st_parent,
                    stip_deep_copies_type *copies)
{
  slice_index i;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  for (i = 0; i!=max_nr_slices; ++i)
    (*copies)[i] = no_slice;

  if (st_parent==0)
    stip_structure_traversal_init(st,copies);
  else
    stip_structure_traversal_init_nested(st,st_parent,copies);

  stip_structure_traversal_override_by_structure(st,
                                                 slice_structure_leaf,
                                                 &deep_copy_leaf);
  stip_structure_traversal_override_by_structure(st,
                                                 slice_structure_pipe,
                                                 &deep_copy_pipe);
  stip_structure_traversal_override_by_structure(st,
                                                 slice_structure_branch,
                                                 &deep_copy_pipe);
  stip_structure_traversal_override_by_structure(st,
                                                 slice_structure_fork,
                                                 &deep_copy_fork);
  stip_structure_traversal_override_by_function(st,
                                                slice_function_binary,
                                                &deep_copy_binary);
  stip_structure_traversal_override_single(st,
                                           STIfThenElse,
                                           &deep_copy_zigzag_jump);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* in-place deep copying a stipulation sub-tree
 * @param si root of sub-tree
 * @return index of root of copy
 */
slice_index stip_deep_copy(slice_index si)
{
  stip_deep_copies_type copies;
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  init_deep_copy(&st,0,&copies);
  stip_traverse_structure(si,&st);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",copies[si]);
  TraceFunctionResultEnd();
  return copies[si];
}

static void hack_fork_apply_setplay(slice_index si, stip_structure_traversal *st)
{
  spin_off_state_type * const state = st->param;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  stip_traverse_structure_children_pipe(si,st);
  TraceValue("%u\n",state->spun_off[slices[si].next1]);

  state->spun_off[si] = state->spun_off[slices[si].next1];
  TraceValue("%u\n",state->spun_off[si]);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}


static structure_traversers_visitor setplay_appliers[] =
{
  { STMoveInverter,      &pipe_spin_off_copy           },
  { STAttackAdapter,     &attack_adapter_apply_setplay },
  { STDefenseAdapter,    &stip_structure_visitor_noop  },
  { STHelpAdapter,       &help_adapter_apply_setplay   },
  { STTemporaryHackFork, &hack_fork_apply_setplay      }
};

enum
{
  nr_setplay_appliers = (sizeof setplay_appliers / sizeof setplay_appliers[0])
};

/* Combine the set play slices into the current stipulation
 * @param setplay slice index of set play
 */
static void insert_set_play(slice_index si, slice_index setplay_slice)
{
  slice_index proxy;
  slice_index regular;
  slice_index const next = slices[si].next1;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",setplay_slice);
  TraceFunctionParamListEnd();

  proxy = alloc_proxy_slice();
  link_to_branch(proxy,setplay_slice);

  if (slices[next].prev==si)
    regular = next;
  else
  {
    regular = alloc_proxy_slice();
    pipe_set_successor(regular,next);
  }

  pipe_link(si,regular);

  {
    slice_index const set_fork = alloc_setplay_fork_slice(proxy);
    branch_insert_slices(si,&set_fork,1);
  }

  pipe_append(proxy,alloc_move_inverter_slice());

  TraceFunctionExit(__func__);
  TraceFunctionParamListEnd();
}

/* Attempt to add set play to the stipulation
 * @param si identifies the root from which to apply set play
 * @return true iff set play could be added
 */
boolean stip_apply_setplay(slice_index si)
{
  boolean result;
  spin_off_state_type state;
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  TraceStipulation(si);

  spin_off_state_init(&state);
  stip_structure_traversal_init(&st,&state);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_pipe,
                                                 &pipe_spin_off_skip);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_branch,
                                                 &pipe_spin_off_skip);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_fork,
                                                 &pipe_spin_off_skip);
  stip_structure_traversal_override(&st,setplay_appliers,nr_setplay_appliers);
  stip_traverse_structure(si,&st);

  if (state.spun_off[si]==no_slice)
    result = false;
  else
  {
    insert_set_play(si,state.spun_off[si]);
    result = true;
  }

  TraceFunctionExit(__func__);
  TraceFunctionParam("%u",result);
  TraceFunctionParamListEnd();
  return result;
}

typedef struct
{
    goal_type const goal_type;
    boolean result;
} goal_search;

static void ends_in_goal(slice_index si, stip_structure_traversal *st)
{
  goal_search * const search = st->param;
  goal_type const goal = slices[si].u.goal_handler.goal.type;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  search->result = search->result || search->goal_type==goal;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Does the current stipulation end in a specific goal?
 * @param si identifies slice where to start
 * @param goal identifies the goal
 * @return true iff one of the goals of the stipulation is goal
 */
boolean stip_ends_in(slice_index si, goal_type goal)
{
  goal_search search = { goal, false };
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",goal);
  TraceFunctionParamListEnd();

  stip_structure_traversal_init(&st,&search);
  stip_structure_traversal_override_single(&st,
                                           STGoalReachedTester,
                                           &ends_in_goal);
  stip_traverse_structure(si,&st);

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",search.result);
  TraceFunctionResultEnd();
  return search.result;
}

static structure_traversers_visitor starter_detectors[] =
{
  { STMovePlayed,     &move_played_detect_starter   },
  { STHelpMovePlayed, &move_played_detect_starter   },
  { STMoveInverter,   &move_inverter_detect_starter }
};

enum
{
  nr_starter_detectors = (sizeof starter_detectors
                          / sizeof starter_detectors[0])
};

/* Detect the starting side from the stipulation
 * @param si identifies slice whose starter to find
 */
void stip_detect_starter(slice_index si)
{
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  TraceStipulation(si);

  stip_structure_traversal_init(&st,NULL);
  stip_structure_traversal_override_by_function(&st,
                                                slice_function_binary,
                                                &binary_detect_starter);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_fork,
                                                 &fork_detect_starter);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_pipe,
                                                 &pipe_detect_starter);
  stip_structure_traversal_override_by_function(&st,
                                                slice_function_testing_pipe,
                                                &pipe_detect_starter);
  stip_structure_traversal_override_by_function(&st,
                                                slice_function_conditional_pipe,
                                                &pipe_detect_starter);
  stip_structure_traversal_override_by_structure(&st,
                                                 slice_structure_branch,
                                                 &pipe_detect_starter);
  stip_structure_traversal_override(&st,
                                    starter_detectors,
                                    nr_starter_detectors);
  stip_traverse_structure(si,&st);

  TraceStipulation(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Impose the starting side on a stipulation
 * @param si identifies slice
 * @param st address of structure that holds the state of the traversal
 */
static void default_impose_starter(slice_index si,
                                   stip_structure_traversal *st)
{
  Side const * const starter = st->param;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceEnumerator(Side,*starter,"");
  TraceFunctionParamListEnd();

  slices[si].starter = *starter;
  stip_traverse_structure_children(si,st);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Impose the starting side on a stipulation.
 * @param si identifies slice
 * @param st address of structure that holds the state of the traversal
 */
static void impose_inverted_starter(slice_index si,
                                    stip_structure_traversal *st)
{
  Side * const starter = st->param;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParam("%u",*starter);
  TraceFunctionParamListEnd();

  slices[si].starter = *starter;

  *starter = advers(*starter);
  stip_traverse_structure_children_pipe(si,st);
  *starter = slices[si].starter;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Slice types that change the starting side
 */
static slice_type starter_inverters[] =
{
  STMovePlayed,
  STHelpMovePlayed,
  STMoveInverter
};

enum
{
  nr_starter_inverters = (sizeof starter_inverters
                          / sizeof starter_inverters[0])
};

static void stip_impose_starter_impl(slice_index si,
                                     Side starter,
                                     stip_structure_traversal *st)
{
  unsigned int i;
  slice_type type;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceEnumerator(Side,starter,"");
  TraceFunctionParamListEnd();

  for (type = 0; type!=nr_slice_types; ++type)
    stip_structure_traversal_override_single(st,
                                             type,
                                             &default_impose_starter);

  for (i = 0; i!=nr_starter_inverters; ++i)
    stip_structure_traversal_override_single(st,
                                             starter_inverters[i],
                                             &impose_inverted_starter);
  stip_structure_traversal_override_single(st,
                                           STIntelligentMateFilter,
                                           &impose_starter_intelligent_mate_filter);
  stip_structure_traversal_override_single(st,
                                           STIntelligentStalemateFilter,
                                           &impose_starter_intelligent_stalemate_filter);
  stip_structure_traversal_override_single(st,
                                           STGoalImmobileReachedTester,
                                           &impose_starter_goal_immobile_tester);

  stip_traverse_structure(si,st);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Set the starting side of the stipulation
 * @param si identifies slice where to start
 * @param starter starting side at the root of the stipulation
 */
void stip_impose_starter(slice_index si, Side starter)
{
  stip_structure_traversal st;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceEnumerator(Side,starter,"");
  TraceFunctionParamListEnd();

  TraceStipulation(si);

  stip_structure_traversal_init(&st,&starter);
  stip_impose_starter_impl(si,starter,&st);
  stip_traverse_structure(si,&st);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Set the starting side of the stipulation from within an ongoing iteration
 * @param si identifies slice where to start
 * @param starter starting side at the root of the stipulation
 * @param st address of structure representing ongoing iteration
 */
void stip_impose_starter_nested(slice_index si,
                                Side starter,
                                stip_structure_traversal *st)
{
  stip_structure_traversal st_nested;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceEnumerator(Side,starter,"");
  TraceFunctionParamListEnd();

  TraceStipulation(si);

  stip_structure_traversal_init_nested(&st_nested,st,&starter);
  stip_impose_starter_impl(si,starter,&st_nested);
  stip_traverse_structure(si,&st_nested);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
