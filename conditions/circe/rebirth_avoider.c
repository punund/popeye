#include "conditions/circe/rebirth_avoider.h"
#include "stipulation/proxy.h"
#include "stipulation/pipe.h"
#include "stipulation/branch.h"
#include "stipulation/fork.h"
#include "stipulation/structure_traversal.h"
#include "stipulation/battle_play/branch.h"
#include "stipulation/help_play/branch.h"
#include "debugging/trace.h"

#include <assert.h>

static void insert_landing(slice_index si, stip_structure_traversal *st)
{
  slice_index const prototype = alloc_pipe(STLandingAfterCirceRebirthHandler);
  branch_insert_slices_contextual(si,st->context,&prototype,1);
}

typedef struct
{
    slice_index landing;
    slice_type type;
} insertion_state_type;

static void insert_fork(slice_index si, stip_structure_traversal *st)
{
  insertion_state_type const * const state = st->param;
  slice_index const proxy = alloc_proxy_slice();
  slice_index const prototype = alloc_fork_slice(state->type,proxy);

  assert(state->landing!=no_slice);
  link_to_branch(proxy,state->landing);

  branch_insert_slices_contextual(si,st->context,&prototype,1);
}

static void instrument_move(slice_index si, stip_structure_traversal *st)
{
  insertion_state_type * const state = st->param;
  slice_index const save_landing = state->landing;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  state->landing = no_slice;
  insert_landing(si,st);

  stip_traverse_structure_children(si,st);

  insert_fork(si,st);
  state->landing = save_landing;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static void remember_landing(slice_index si, stip_structure_traversal *st)
{
  insertion_state_type * const state = st->param;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  assert(state->landing==no_slice);
  stip_traverse_structure_children_pipe(si,st);
  assert(state->landing==no_slice);
  state->landing = si;

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Instrument a stipulation with a type of "Circe rebirth avoiders" (i.e.
 * slices that may detour around Circe rebirth under a certain condition;
 * STCaptureFork is an example).
 * @param si identifies root slice of stipulation
 * @param type tye of Circe rebirth avoider
 */
void stip_insert_rebirth_avoider(slice_index si, slice_type type)
{
  stip_structure_traversal st;
  insertion_state_type state = { no_slice, type };

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  stip_structure_traversal_init(&st,&state);
  stip_structure_traversal_override_single(&st,
                                           STMove,
                                           &instrument_move);
  stip_structure_traversal_override_single(&st,
                                           STLandingAfterCirceRebirthHandler,
                                           &remember_landing);
  stip_structure_traversal_override_single(&st,
                                           STCageCirceFutileCapturesRemover,
                                           &stip_structure_visitor_noop);
  stip_traverse_structure(si,&st);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}