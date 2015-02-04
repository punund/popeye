#include "conditions/eiffel.h"
#include "solving/move_generator.h"
#include "solving/observation.h"
#include "solving/find_square_observer_tracking_back_from_target.h"
#include "solving/pipe.h"
#include "stipulation/stipulation.h"
#include "pieces/pieces.h"
#include "position/position.h"

#include "debugging/trace.h"

static piece_walk_type get_paralyser(piece_walk_type p)
{
  piece_walk_type result = Empty;

  TraceFunctionEntry(__func__);
  TraceWalk(p);
  TraceFunctionParamListEnd();

  switch (p)
  {
    case Pawn:
      result = Queen;
      break;

    case Queen:
      result = Rook;
      break;

    case Rook:
      result = Bishop;
      break;

    case Bishop:
      result = Knight;
      break;

    case Knight:
      result = Pawn;
      break;

    default:
      break;
  }

  TraceFunctionExit(__func__);
  TraceWalk(result);
  TraceFunctionResultEnd();
  return result;
}

static boolean is_paralysed(numecoup n)
{
  square const sq_departure = move_generation_stack[n].departure;
  piece_walk_type const p = get_walk_of_piece_on_square(sq_departure);
  boolean result = false;
  piece_walk_type eiffel_piece;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  eiffel_piece = get_paralyser(p);

  if (eiffel_piece!=Empty)
  {
    Side const eiffel_side = advers(trait[nbply]);
    if (being_solved.number_of_pieces[eiffel_side][eiffel_piece]>0)
    {
      siblingply(eiffel_side);
      push_observation_target(sq_departure);
      observing_walk[nbply] = eiffel_piece;
      result = is_square_observed_nested(SLICE_NEXT2(temporary_hack_is_square_observed_specific[trait[nbply]]),
                                         EVALUATE(observation_geometry));
      finply();
    }
  }

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Generate moves for a single piece
 * @param identifies generator slice
 */
void eiffel_generate_moves_for_piece(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  if (!is_paralysed(current_generation))
    pipe_move_generation_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

/* Validate an observater according to Eiffel Chess
 * @return true iff the observation is valid
 */
boolean eiffel_validate_observer(slice_index si)
{
  boolean result = true;

  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  result = (!is_paralysed(CURRMOVE_OF_PLY(nbply))
            && validate_observation_recursive(SLICE_NEXT1(si)));

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

/* Inialise the solving machinery with Eiffel Chess
 * @param si identifies root slice of solving machinery
 */
void eiffel_initialise_solving(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  solving_instrument_move_generation(si,nr_sides,STEiffelMovesForPieceGenerator);

  stip_instrument_observer_validation(si,nr_sides,STEiffelMovesForPieceGenerator);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
