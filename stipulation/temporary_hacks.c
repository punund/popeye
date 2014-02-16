#include "stipulation/temporary_hacks.h"
#include "conditions/conditions.h"
#include "stipulation/pipe.h"
#include "stipulation/has_solution_type.h"
#include "stipulation/branch.h"
#include "stipulation/proxy.h"
#include "stipulation/end_of_branch.h"
#include "stipulation/conditional_pipe.h"
#include "stipulation/move_inverter.h"
#include "stipulation/boolean/true.h"
#include "stipulation/boolean/false.h"
#include "stipulation/boolean/not.h"
#include "stipulation/boolean/or.h"
#include "stipulation/goals/reached_tester.h"
#include "stipulation/goals/mate/reached_tester.h"
#include "stipulation/goals/immobile/reached_tester.h"
#include "stipulation/goals/any/reached_tester.h"
#include "stipulation/goals/capture/reached_tester.h"
#include "stipulation/goals/king_capture/reached_tester.h"
#include "stipulation/battle_play/branch.h"
#include "stipulation/help_play/branch.h"
#include "solving/legal_move_counter.h"
#include "conditions/sat.h"
#include "optimisations/count_nr_opponent_moves/opponent_moves_counter.h"
#include "optimisations/count_nr_opponent_moves/move_generator.h"
#include "debugging/trace.h"

#include "debugging/assert.h"

slice_index temporary_hack_mate_tester[nr_sides];
slice_index temporary_hack_exclusive_mating_move_counter[nr_sides];
slice_index temporary_hack_brunner_check_defense_finder[nr_sides];
slice_index temporary_hack_ultra_mummer_length_measurer[nr_sides];
slice_index temporary_hack_move_legality_tester[nr_sides];
slice_index temporary_hack_king_capture_legality_tester[nr_sides];
slice_index temporary_hack_cagecirce_noncapture_finder[nr_sides];
slice_index temporary_hack_circe_take_make_rebirth_squares_finder[nr_sides];
slice_index temporary_hack_castling_intermediate_move_legality_tester[nr_sides];
slice_index temporary_hack_opponent_moves_counter[nr_sides];
slice_index temporary_hack_back_home_finder[nr_sides];
slice_index temporary_hack_suffocation_by_paralysis_finder[nr_sides];
slice_index temporary_hack_move_generator[nr_sides];
slice_index temporary_hack_is_square_observed[nr_sides];
slice_index temporary_hack_is_square_observed_specific[nr_sides];
slice_index temporary_hack_is_square_observed_by_non_king[nr_sides];
slice_index temporary_hack_check_validator[nr_sides];
slice_index temporary_hack_observation_validator[nr_sides];
slice_index temporary_hack_observer_validator[nr_sides];
slice_index temporary_hack_observation_geometry_validator[nr_sides];
slice_index temporary_hack_check_tester;

static slice_index make_mate_tester_fork(Side side)
{
  slice_index result;

  if (CondFlag[exclusive] || CondFlag[republican])
  {
    Goal const mate_goal = { goal_mate, initsquare };
    slice_index const mate_tester = alloc_goal_mate_reached_tester_system();
    result = alloc_goal_reached_tester_slice(mate_goal,mate_tester);
    dealloc_slice(slices[result].next1);
    stip_impose_starter(result,side);
  }
  else
    result = alloc_proxy_slice();

  return result;
}

static slice_index make_exclusive_mating_move_counter_fork(Side side)
{
  slice_index result;
  slice_index const proxy_branch = alloc_proxy_slice();
  slice_index const proxy_to_goal = alloc_proxy_slice();
  Goal const goal = { goal_mate, initsquare };
  slice_index const tester_system = alloc_goal_mate_reached_tester_system();
  slice_index const tester_slice = alloc_goal_reached_tester_slice(goal,tester_system);
  slice_index const attack = alloc_battle_branch(slack_length+1,slack_length+1);
  slice_index const counter = alloc_pipe(STExclusiveChessGoalReachingMoveCounter);
  link_to_branch(counter,tester_slice);
  link_to_branch(proxy_to_goal,counter);
  link_to_branch(proxy_branch,attack);
  battle_branch_insert_direct_end_of_branch_goal(attack,proxy_to_goal);
  result = alloc_conditional_pipe(STExclusiveChessMatingMoveCounterFork,proxy_branch);
  stip_impose_starter(result,side);
  return result;
}

static slice_index make_brunner_check_defense_finder(Side side)
{
  slice_index result;
  slice_index const proxy_branch = alloc_proxy_slice();
  slice_index const help = alloc_help_branch(slack_length+1,slack_length+1);
  slice_index const proxy_goal = alloc_proxy_slice();
  slice_index const system = alloc_goal_any_reached_tester_system();
  link_to_branch(proxy_goal,system);
  help_branch_set_end_goal(help,proxy_goal,1);
  link_to_branch(proxy_branch,help);
  result = alloc_conditional_pipe(STBrunnerDefenderFinder,proxy_branch);
  stip_impose_starter(result,side);
  return result;
}

static slice_index make_move_legality_tester(Side side)
{
  slice_index result;
  slice_index const proxy_branch = alloc_proxy_slice();
  slice_index const help = alloc_help_branch(slack_length+1,slack_length+1);
  slice_index const proxy_goal = alloc_proxy_slice();
  slice_index const system = alloc_goal_any_reached_tester_system();
  link_to_branch(proxy_goal,system);
  help_branch_set_end_goal(help,proxy_goal,1);
  link_to_branch(proxy_branch,help);
  result = alloc_conditional_pipe(STMoveLegalityTester,proxy_branch);
  stip_impose_starter(result,side);
  return result;
}

static slice_index make_king_capture_legality_tester(Side side)
{
  slice_index result;
  slice_index const proxy_branch = alloc_proxy_slice();
  slice_index const help = alloc_help_branch(slack_length+1,slack_length+1);
  slice_index const proxy_goal = alloc_proxy_slice();
  slice_index const system = alloc_goal_king_capture_reached_tester_system();
  link_to_branch(proxy_goal,system);
  help_branch_set_end_goal(help,proxy_goal,1);
  link_to_branch(proxy_branch,help);
  result = alloc_conditional_pipe(STKingCaptureLegalityTester,proxy_branch);
  stip_impose_starter(result,side);
  return result;
}

static slice_index make_ultra_mummer_length_measurer(Side side)
{
  slice_index result;
  slice_index const proxy_branch = alloc_proxy_slice();
  slice_index const help = alloc_help_branch(slack_length+1,slack_length+1);
  slice_index const proxy_goal = alloc_proxy_slice();
  slice_index const system = alloc_goal_any_reached_tester_system();
  link_to_branch(proxy_goal,system);
  help_branch_set_end_goal(help,proxy_goal,1);
  link_to_branch(proxy_branch,help);
  result = alloc_conditional_pipe(STUltraMummerMeasurerFork,proxy_branch);
  stip_impose_starter(result,side);
  return result;
}

static slice_index make_cagecirce_noncapture_finder(Side side)
{
  slice_index result;

  if (CondFlag[circecage])
  {
    slice_index const proxy_branch = alloc_proxy_slice();
    slice_index const help = alloc_help_branch(slack_length+1,slack_length+1);
    slice_index const proxy_goal = alloc_proxy_slice();
    slice_index const system = alloc_goal_capture_reached_tester_system();
    link_to_branch(proxy_goal,system);

    {
      slice_index const tester = branch_find_slice(STGoalReachedTester,
                                                   proxy_goal,
                                                   stip_traversal_context_intro);
      assert(tester!=no_slice);
      pipe_append(slices[tester].next2,alloc_not_slice());
      slices[tester].u.goal_handler.goal.type = goal_negated;
      help_branch_set_end_goal(help,proxy_goal,1);
      link_to_branch(proxy_branch,help);
      result = alloc_conditional_pipe(STCageCirceNonCapturingMoveFinder,proxy_branch);
      stip_impose_starter(result,side);
    }
  }
  else
    result = alloc_proxy_slice();

  return result;
}

static slice_index make_circe_take_make_rebirth_squares_finder(Side side)
{
  slice_index result;
  slice_index const proxy_branch = alloc_proxy_slice();
  slice_index const help = alloc_help_branch(slack_length+1,slack_length+1);
  slice_index const prototype = alloc_pipe(STTakeMakeCirceCollectRebirthSquares);
  link_to_branch(proxy_branch,help);
  help_branch_insert_slices(help,&prototype,1);
  result = alloc_conditional_pipe(STTakeMakeCirceCollectRebirthSquaresFork,proxy_branch);
  stip_impose_starter(result,side);

  return result;
}

static slice_index make_castling_intermediate_move_legality_tester(Side side)
{
  slice_index result;
  slice_index const proxy_branch = alloc_proxy_slice();
  slice_index const help = alloc_help_branch(slack_length+1,slack_length+1);
  slice_index const proxy_goal = alloc_proxy_slice();
  slice_index const system = alloc_goal_any_reached_tester_system();
  link_to_branch(proxy_goal,system);
  help_branch_set_end_goal(help,proxy_goal,1);
  link_to_branch(proxy_branch,help);
  result = alloc_conditional_pipe(STCastlingIntermediateMoveLegalityTester,proxy_branch);
  stip_impose_starter(result,side);

  return result;
}

static slice_index make_opponent_moves_counter_fork(Side side)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceEnumerator(Side,side,"");
  TraceFunctionParamListEnd();

  if (is_countnropponentmoves_defense_move_optimisation_enabled(side))
  {
    slice_index const proxy = alloc_proxy_slice();
    slice_index const prototypes[] =
    {
        alloc_pipe(STOpponentMovesCounter),
        alloc_legal_attack_counter_slice()
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_index const attack = alloc_defense_branch(slack_length+2,slack_length+1);
    branch_insert_slices(attack,prototypes,nr_prototypes);
    link_to_branch(proxy,attack);
    result = alloc_conditional_pipe(STOpponentMovesCounterFork,proxy);
    stip_impose_starter(result,side);
  }
  else
    result = alloc_proxy_slice();

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

static slice_index make_back_home_finder(Side side)
{
  slice_index const proxy = alloc_proxy_slice();
  slice_index const result = alloc_conditional_pipe(STBackHomeFinderFork,proxy);
  slice_index const defense = alloc_defense_branch(slack_length+1,slack_length+1);
  slice_index const prototypes[] =
  {
      alloc_pipe(STBackHomeMovesOnly),
      alloc_legal_defense_counter_slice()
  };
  branch_insert_slices(defense,prototypes,2);
  link_to_branch(proxy,defense);
  stip_impose_starter(result,side);
  return result;
}

static slice_index make_suffocation_by_paralysis_detector(Side side)
{
  slice_index const proxy = alloc_proxy_slice();
  slice_index const result = alloc_conditional_pipe(STPiecesParalysingSuffocationFinderFork,proxy);
  slice_index const attack = alloc_battle_branch(slack_length+1,slack_length+1);
  slice_index const prototype = alloc_pipe(STPiecesParalysingSuffocationFinder);
  branch_insert_slices(attack,&prototype,1);
  link_to_branch(proxy,attack);
  stip_impose_starter(result,side);
  return result;
}

static slice_index make_move_generator(Side side)
{
  slice_index const proxy = alloc_proxy_slice();
  slice_index const result = alloc_conditional_pipe(STMoveGeneratorFork,proxy);
  slice_index const generating = alloc_pipe(STGeneratingMovesForPiece);
  slice_index const ortho = alloc_pipe(STMovesForPieceBasedOnWalkGenerator);
  pipe_append(proxy,generating);
  pipe_append(generating,ortho);
  pipe_link(ortho,alloc_true_slice());
  stip_impose_starter(result,side);
  return result;
}

static slice_index make_check_tester(void)
{
  slice_index const proxy = alloc_proxy_slice();
  slice_index const result = alloc_conditional_pipe(STCheckTesterFork,proxy);
  slice_index const testing = alloc_pipe(STTestingCheck);
  slice_index const initialiser = alloc_pipe(STKingSquareObservationTesterPlyInitialiser);
  slice_index const king_square_observation_tester = alloc_pipe(STKingSquareObservationTester);
  link_to_branch(proxy,testing);
  pipe_append(testing,initialiser);
  pipe_append(initialiser,king_square_observation_tester);
  pipe_link(king_square_observation_tester,alloc_true_slice());
  stip_impose_starter(result,Black);
  return result;
}

static void make_is_square_observed(Side side)
{
  slice_index const proxy = alloc_proxy_slice();
  slice_index const testing = alloc_pipe(STTestingIfSquareIsObserved);
  slice_index const determine_walk = alloc_pipe(STDetermineObserverWalk);
  slice_index const testing_specific = alloc_pipe(STTestingIfSquareIsObservedWithSpecificWalk);

  slice_index const optimising = alloc_pipe(STOptimisingObserverWalk);
  slice_index const track_back = alloc_pipe(STTrackBackFromTargetAccordingToObserverWalk);

  temporary_hack_is_square_observed[side] = alloc_conditional_pipe(STIsSquareObservedFork,proxy);
  temporary_hack_is_square_observed_specific[side] = alloc_conditional_pipe(STIsSquareObservedFork,testing_specific);

  pipe_link(proxy,testing);
  pipe_link(testing,determine_walk);
  pipe_link(determine_walk,testing_specific);
  pipe_link(testing_specific,optimising);
  pipe_link(optimising,track_back);
  pipe_link(track_back,alloc_false_slice());

  stip_impose_starter(temporary_hack_is_square_observed[side],side);
  stip_impose_starter(temporary_hack_is_square_observed_specific[side],side);
}

static slice_index make_check_validator(Side side)
{
  slice_index const proxy = alloc_proxy_slice();
  slice_index const result = alloc_conditional_pipe(STValidatingCheckFork,proxy);
  slice_index const check = alloc_pipe(STValidatingCheck);
  slice_index const enforce_walk = alloc_pipe(STEnforceObserverWalk);
  slice_index const observer = alloc_pipe(STValidatingObserver);
  slice_index const geometry = alloc_pipe(STValidatingObservationGeometry);

  pipe_link(proxy,check);
  pipe_link(check,enforce_walk);
  pipe_link(enforce_walk,observer);
  pipe_link(observer,geometry);
  pipe_link(geometry,alloc_true_slice());
  stip_impose_starter(result,side);

  return result;
}

static slice_index make_observation_validator(Side side)
{
  slice_index const proxy = alloc_proxy_slice();
  slice_index const result = alloc_conditional_pipe(STValidatingObservationFork,proxy);
  slice_index const observation = alloc_pipe(STValidatingObservation);
  slice_index const enforce_walk = alloc_pipe(STEnforceObserverWalk);
  slice_index const observer = alloc_pipe(STValidatingObserver);
  slice_index const geometry = alloc_pipe(STValidatingObservationGeometry);

  pipe_link(proxy,observation);
  pipe_link(observation,enforce_walk);
  pipe_link(enforce_walk,observer);
  pipe_link(observer,geometry);
  pipe_link(geometry,alloc_true_slice());
  stip_impose_starter(result,side);

  return result;
}

static slice_index make_observer_validator(Side side)
{
  slice_index const proxy = alloc_proxy_slice();
  slice_index const result = alloc_conditional_pipe(STValidatingObserverFork,proxy);
  slice_index const observer = alloc_pipe(STValidatingObserver);
  slice_index const enforce_walk = alloc_pipe(STEnforceObserverWalk);
  slice_index const geometry = alloc_pipe(STValidatingObservationGeometry);

  pipe_link(proxy,observer);
  pipe_link(observer,enforce_walk);
  pipe_link(enforce_walk,geometry);
  pipe_link(geometry,alloc_true_slice());
  stip_impose_starter(result,side);

  return result;
}

static slice_index make_observation_geometry_validator(Side side)
{
  slice_index const proxy = alloc_proxy_slice();
  slice_index const result = alloc_conditional_pipe(STValidatingObservationGeometryFork,proxy);
  slice_index const enforce_walk = alloc_pipe(STEnforceObserverWalk);
  slice_index const testing = alloc_pipe(STValidatingObservationGeometry);

  pipe_link(proxy,testing);
  pipe_link(testing,enforce_walk);
  pipe_link(enforce_walk,alloc_true_slice());
  stip_impose_starter(result,side);

  return result;
}

void insert_temporary_hacks(slice_index root_slice)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",root_slice);
  TraceFunctionParamListEnd();

  {
    slice_index const proxy = alloc_proxy_slice();
    slice_index const entry_point = alloc_conditional_pipe(STTemporaryHackFork,proxy);

    slice_index const inverter = alloc_move_inverter_slice();

    pipe_link(proxy,alloc_true_slice());

    temporary_hack_mate_tester[Black] = make_mate_tester_fork(Black);
    temporary_hack_mate_tester[White] = make_mate_tester_fork(White);

    temporary_hack_exclusive_mating_move_counter[Black] = make_exclusive_mating_move_counter_fork(Black);
    temporary_hack_exclusive_mating_move_counter[White] = make_exclusive_mating_move_counter_fork(White);

    temporary_hack_brunner_check_defense_finder[Black] = make_brunner_check_defense_finder(Black);
    temporary_hack_brunner_check_defense_finder[White] = make_brunner_check_defense_finder(White);

    temporary_hack_ultra_mummer_length_measurer[Black] = make_ultra_mummer_length_measurer(Black);
    temporary_hack_ultra_mummer_length_measurer[White] = make_ultra_mummer_length_measurer(White);

    temporary_hack_move_legality_tester[Black] = make_move_legality_tester(Black);
    temporary_hack_move_legality_tester[White] = make_move_legality_tester(White);

    temporary_hack_king_capture_legality_tester[Black] = make_king_capture_legality_tester(Black);
    temporary_hack_king_capture_legality_tester[White] = make_king_capture_legality_tester(White);

    temporary_hack_cagecirce_noncapture_finder[Black] = make_cagecirce_noncapture_finder(Black);
    temporary_hack_cagecirce_noncapture_finder[White] = make_cagecirce_noncapture_finder(White);

    temporary_hack_circe_take_make_rebirth_squares_finder[Black] = make_circe_take_make_rebirth_squares_finder(Black);
    temporary_hack_circe_take_make_rebirth_squares_finder[White] = make_circe_take_make_rebirth_squares_finder(White);

    temporary_hack_castling_intermediate_move_legality_tester[Black] = make_castling_intermediate_move_legality_tester(Black);
    temporary_hack_castling_intermediate_move_legality_tester[White] = make_castling_intermediate_move_legality_tester(White);

    temporary_hack_opponent_moves_counter[Black] = make_opponent_moves_counter_fork(Black);
    temporary_hack_opponent_moves_counter[White] = make_opponent_moves_counter_fork(White);

    temporary_hack_back_home_finder[Black] = make_back_home_finder(Black);
    temporary_hack_back_home_finder[White] = make_back_home_finder(White);

    temporary_hack_suffocation_by_paralysis_finder[Black] = make_suffocation_by_paralysis_detector(Black);
    temporary_hack_suffocation_by_paralysis_finder[White] = make_suffocation_by_paralysis_detector(White);

    temporary_hack_move_generator[Black] = make_move_generator(Black);
    temporary_hack_move_generator[White] = make_move_generator(White);

    make_is_square_observed(Black);
    make_is_square_observed(White);

    temporary_hack_check_validator[Black] = make_check_validator(Black);
    temporary_hack_check_validator[White] = make_check_validator(White);

    temporary_hack_observation_validator[Black] = make_observation_validator(Black);
    temporary_hack_observation_validator[White] = make_observation_validator(White);

    temporary_hack_observer_validator[Black] = make_observer_validator(Black);
    temporary_hack_observer_validator[White] = make_observer_validator(White);

    temporary_hack_observation_geometry_validator[Black] = make_observation_geometry_validator(Black);
    temporary_hack_observation_geometry_validator[White] = make_observation_geometry_validator(White);

    temporary_hack_check_tester = make_check_tester();

    pipe_append(root_slice,entry_point);

    pipe_append(proxy,temporary_hack_mate_tester[White]);
    pipe_append(temporary_hack_mate_tester[White],
                temporary_hack_exclusive_mating_move_counter[White]);
    pipe_append(temporary_hack_exclusive_mating_move_counter[White],
                temporary_hack_brunner_check_defense_finder[White]);
    pipe_append(temporary_hack_brunner_check_defense_finder[White],
                temporary_hack_ultra_mummer_length_measurer[White]);
    pipe_append(temporary_hack_ultra_mummer_length_measurer[White],
                temporary_hack_move_legality_tester[White]);
    pipe_append(temporary_hack_move_legality_tester[White],
                temporary_hack_king_capture_legality_tester[White]);
    pipe_append(temporary_hack_king_capture_legality_tester[White],
                temporary_hack_cagecirce_noncapture_finder[White]);
    pipe_append(temporary_hack_cagecirce_noncapture_finder[White],
                temporary_hack_circe_take_make_rebirth_squares_finder[White]);
    pipe_append(temporary_hack_circe_take_make_rebirth_squares_finder[White],
                temporary_hack_castling_intermediate_move_legality_tester[White]);
    pipe_append(temporary_hack_castling_intermediate_move_legality_tester[White],
                temporary_hack_opponent_moves_counter[White]);
    pipe_append(temporary_hack_opponent_moves_counter[White],
                temporary_hack_back_home_finder[White]);
    pipe_append(temporary_hack_back_home_finder[White],
                temporary_hack_suffocation_by_paralysis_finder[White]);
    pipe_append(temporary_hack_suffocation_by_paralysis_finder[White],
                temporary_hack_move_generator[White]);
    pipe_append(temporary_hack_move_generator[White],
                temporary_hack_is_square_observed_specific[White]);
    pipe_append(temporary_hack_is_square_observed_specific[White],
                temporary_hack_is_square_observed[White]);
    pipe_append(temporary_hack_is_square_observed[White],
                temporary_hack_check_validator[White]);
    pipe_append(temporary_hack_check_validator[White],
                temporary_hack_observation_validator[White]);
    pipe_append(temporary_hack_observation_validator[White],
                temporary_hack_observation_geometry_validator[White]);
    pipe_append(temporary_hack_observation_geometry_validator[White],
                temporary_hack_observer_validator[White]);
    pipe_append(temporary_hack_observer_validator[White],
                inverter);

    pipe_append(inverter,temporary_hack_mate_tester[Black]);
    pipe_append(temporary_hack_mate_tester[Black],
                temporary_hack_exclusive_mating_move_counter[Black]);
    pipe_append(temporary_hack_exclusive_mating_move_counter[Black],
                temporary_hack_brunner_check_defense_finder[Black]);
    pipe_append(temporary_hack_brunner_check_defense_finder[Black],
                temporary_hack_ultra_mummer_length_measurer[Black]);
    pipe_append(temporary_hack_ultra_mummer_length_measurer[Black],
                temporary_hack_move_legality_tester[Black]);
    pipe_append(temporary_hack_move_legality_tester[Black],
                temporary_hack_king_capture_legality_tester[Black]);
    pipe_append(temporary_hack_king_capture_legality_tester[Black],
                temporary_hack_cagecirce_noncapture_finder[Black]);
    pipe_append(temporary_hack_cagecirce_noncapture_finder[Black],
                temporary_hack_circe_take_make_rebirth_squares_finder[Black]);
    pipe_append(temporary_hack_circe_take_make_rebirth_squares_finder[Black],
                temporary_hack_castling_intermediate_move_legality_tester[Black]);
    pipe_append(temporary_hack_castling_intermediate_move_legality_tester[Black],
                temporary_hack_opponent_moves_counter[Black]);
    pipe_append(temporary_hack_opponent_moves_counter[Black],
                temporary_hack_back_home_finder[Black]);
    pipe_append(temporary_hack_back_home_finder[Black],
                temporary_hack_suffocation_by_paralysis_finder[Black]);
    pipe_append(temporary_hack_suffocation_by_paralysis_finder[Black],
                temporary_hack_move_generator[Black]);
    pipe_append(temporary_hack_move_generator[Black],
                temporary_hack_is_square_observed_specific[Black]);
    pipe_append(temporary_hack_is_square_observed_specific[Black],
                temporary_hack_is_square_observed[Black]);
    pipe_append(temporary_hack_is_square_observed[Black],
                temporary_hack_check_validator[Black]);
    pipe_append(temporary_hack_check_validator[Black],
                temporary_hack_observation_validator[Black]);
    pipe_append(temporary_hack_observation_validator[Black],
                temporary_hack_observation_geometry_validator[Black]);
    pipe_append(temporary_hack_observation_geometry_validator[Black],
                temporary_hack_observer_validator[Black]);
    pipe_append(temporary_hack_observer_validator[Black],
                temporary_hack_check_tester);

    if (slices[root_slice].starter==Black)
      pipe_append(proxy,alloc_move_inverter_slice());
  }

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
