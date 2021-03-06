#include "output/plaintext/position.h"
#include "output/plaintext/plaintext.h"
#include "output/plaintext/protocol.h"
#include "output/plaintext/language_dependant.h"
#include "output/plaintext/condition.h"
#include "output/plaintext/pieces.h"
#include "output/plaintext/message.h"
#include "output/plaintext/stdio.h"
#include "output/output.h"
#include "input/plaintext/problem.h"
#include "input/plaintext/stipulation.h"
#include "options/maxthreatlength.h"
#include "options/maxflightsquares.h"
#include "options/nontrivial.h"
#include "conditions/grid.h"
#include "conditions/imitator.h"
#include "pieces/attributes/neutral/neutral.h"
#include "pieces/walks/classification.h"
#include "pieces/walks/hunters.h"
#include "position/position.h"
#include "solving/castling.h"
#include "solving/move_generator.h"
#include "solving/proofgames.h"
#include "solving/pipe.h"
#include "stipulation/pipe.h"
#include "debugging/assert.h"

#include <ctype.h>
#include <string.h>

enum
{
  fileWidth = 4,
  border_width = 5,
  board_width = fileWidth*nr_files_on_board + border_width
};

static boolean is_square_occupied_by_imitator(position const *pos, square s)
{
  boolean result = false;
  unsigned int imi_idx;

  for (imi_idx = 0; imi_idx<pos->number_of_imitators; ++imi_idx)
    if (s==pos->isquare[imi_idx])
    {
      result = true;
      break;
    }

  return result;
}

static void WriteCastlingMutuallyExclusive(void)
{
  /* no need to test in [Black] - information is redundant */
  if (castling_mutual_exclusive[White][queenside_castling-min_castling]!=0
      || castling_mutual_exclusive[White][kingside_castling-min_castling]!=0)
  {
    protocol_fprintf(stdout,"%s",OptTab[mutuallyexclusivecastling]);

    if ((castling_mutual_exclusive[White][queenside_castling-min_castling]
         &ra_cancastle))
    {
      protocol_fputc(' ',stdout);
      WriteSquare(&output_plaintext_engine,stdout,square_a1);
      WriteSquare(&output_plaintext_engine,stdout,square_a8);
    }

    if ((castling_mutual_exclusive[White][queenside_castling-min_castling]
         &rh_cancastle))
    {
      protocol_fputc(' ',stdout);
      WriteSquare(&output_plaintext_engine,stdout,square_a1);
      WriteSquare(&output_plaintext_engine,stdout,square_h8);
    }

    if ((castling_mutual_exclusive[White][kingside_castling-min_castling]
         &ra_cancastle))
    {
      protocol_fputc(' ',stdout);
      WriteSquare(&output_plaintext_engine,stdout,square_h1);
      WriteSquare(&output_plaintext_engine,stdout,square_a8);
    }

    if ((castling_mutual_exclusive[White][kingside_castling-min_castling]
         &rh_cancastle))
    {
      protocol_fputc(' ',stdout);
      WriteSquare(&output_plaintext_engine,stdout,square_h1);
      WriteSquare(&output_plaintext_engine,stdout,square_h8);
    }

    protocol_fputc('\n',stdout);
  }
}

static void WriteGrid(void)
{
  square square, square_a;
  int row, column;
  char    HLine[40];

  static char BorderL[]="+---a---b---c---d---e---f---g---h---+\n";
  static char HorizL[]="%c                                   %c\n";
  static char BlankL[]="|                                   |\n";

  protocol_fputc('\n',stdout);
  protocol_fprintf(stdout,"%s",BorderL);
  protocol_fprintf(stdout,"%s",BlankL);

  for (row=0, square_a = square_a8;
       row<nr_rows_on_board;
       row++, square_a += dir_down) {
    char const *digits="87654321";
    sprintf(HLine, HorizL, digits[row], digits[row]);

    for (column=0, square= square_a;
         column<nr_files_on_board;
         column++, square += dir_right)
    {
      char g = (GridNum(square))%100;
      HLine[fileWidth*column+3]= g>9 ? (g/10)+'0' : ' ';
      HLine[fileWidth*column+4]= (g%10)+'0';
    }

    protocol_fprintf(stdout,"%s",HLine);
    protocol_fprintf(stdout,"%s",BlankL);
  }

  protocol_fprintf(stdout,"%s",BorderL);
}

static void WritePiecesWithAttribute(position const *pos, piece_flag_type sp)
{
  char squares[3*nr_rows_on_board*nr_files_on_board+1] = "";
  square square_a = square_a8;
  unsigned int row;

  for (row = 1; row<=nr_rows_on_board; ++row, square_a += dir_down)
  {
    unsigned int column;
    square square = square_a;

    for (column = 1; column<=nr_files_on_board; ++column, square += dir_right)
      if (TSTFLAG(pos->spec[square],sp))
        AppendSquare(squares,square);
  }

  protocol_fprintf_c(stdout,board_width,"%s%s\n",PieSpTab[sp-nr_sides],squares);
}

static void WriteNonRoyalAttributedPieces(position const *pos)
{
  piece_flag_type sp;

  for (sp = Royal+1; sp<nr_piece_flags; ++sp)
    if (!TSTFLAG(all_pieces_flags,sp) && TSTFLAG(some_pieces_flags,sp))
      WritePiecesWithAttribute(pos,sp);
}

static void WriteRoyalPiecePositions(position const *pos)
{
  char squares[3*nr_rows_on_board*nr_files_on_board+1] = "";
  unsigned int nr_royals = 0;

  square square_a = square_a8;
  unsigned int row;

  for (row = 0; row!=nr_rows_on_board; ++row, square_a += dir_down)
  {
    unsigned int column;
    square square = square_a;

    for (column = 0; column!=nr_files_on_board; ++column, square += dir_right)
      if (TSTFLAG(pos->spec[square],Royal)
          && !is_king(pos->board[square]))
      {
        AppendSquare(squares,square);
        ++nr_royals;
      }
  }

  if (nr_royals>0)
    protocol_fprintf_c(stdout,board_width,"%s%s\n",PieSpTab[Royal-nr_sides],squares);
}

static void DoPieceCounts(position const *pos,
                          unsigned piece_per_colour[nr_colours])
{
  square square_a = square_a8;
  unsigned int row;

  for (row = 0; row!=nr_rows_on_board; ++row, square_a += dir_down)
  {
    unsigned int column;
    square square = square_a;

    for (column = 0; column!=nr_files_on_board; ++column, square += dir_right)
    {
      if (is_piece_neutral(pos->spec[square]))
        ++piece_per_colour[colour_neutral];
      else if (TSTFLAG(pos->spec[square],Black))
        ++piece_per_colour[colour_black];
      else if (TSTFLAG(pos->spec[square],White))
        ++piece_per_colour[colour_white];
    }
  }
}

static int indentation = 0;

static void WritePieceCounts(position const *pos, unsigned int indentation)
{
  unsigned piece_per_colour[nr_colours] = { 0 };

  DoPieceCounts(pos,piece_per_colour);

  {
    char const *format = piece_per_colour[colour_neutral]>0 ? "%d + %d + %dn\n" : "%d + %d\n";
    int const width = nr_files_on_board*fileWidth+4-indentation;
    protocol_fprintf_r(stdout,width,format,
                       piece_per_colour[colour_white],
                       piece_per_colour[colour_black],
                       piece_per_colour[colour_neutral]);
  }
}

static int WriteStipulation(void)
{
  char StipLower[sizeof AlphaStip];
  char *stip = AlphaStip;
  char *lower = StipLower;

  while (*stip)
    *lower++ = tolower(*stip++);
  *lower = 0;

  return protocol_fprintf(stdout,"  %s",StipLower);
}

static int WriteOptions(position const *pos)
{
  int result = 0;

  TraceValue("%u\n",indentation);

  if (OptFlag[solmenaces])
  {
    result += protocol_fprintf(stdout, "/%u", get_max_threat_length());
    if (OptFlag[solflights])
      result += protocol_fprintf(stdout, "/%d", get_max_flights());
  }
  else if (OptFlag[solflights])
    result += protocol_fprintf(stdout, "//%d", get_max_flights());

  if (OptFlag[nontrivial])
    result += protocol_fprintf(stdout,";%d,%u",
                              max_nr_nontrivial,
                              get_min_length_nontrivial());

  return result;
}

static char *WriteWalkRtoL(char *pos, piece_walk_type walk)
{
  pos[0] = PieceTab[walk][1];
  if (pos[0]!=' ')
  {
    pos[0] = toupper(pos[0]);
    --pos;
  }

  pos[0] = toupper(PieceTab[walk][0]);
  --pos;

  return pos;
}

static void WriteRegularCells(position const *pos, square square_a)
{
  unsigned int column;
  square square;

  for (column = 0,  square = square_a;
      column!=nr_files_on_board;
       ++column, square += dir_right)
  {
    char cell[fileWidth+1];
    char *pos_in_cell = cell + (sizeof cell)/2;

    snprintf(cell, sizeof cell, "%*c", fileWidth, ' ');

    if (CondFlag[gridchess] && !OptFlag[suppressgrid])
    {
      if (is_on_board(square+dir_left)
          && GridLegal(square, square+dir_left))
        cell[0] = '|';
    }

    if (is_square_occupied_by_imitator(pos,square))
      pos_in_cell[0] = 'I';
    else if (pos->board[square]==Invalid)
      pos_in_cell[0] = ' ';
    else if (pos->board[square]==Empty)
      pos_in_cell[0] = '.';
    else
    {
      piece_walk_type const walk = pos->board[square];
      if (walk<Hunter0 || walk>=Hunter0+max_nr_hunter_walks)
        pos_in_cell = WriteWalkRtoL(pos_in_cell,walk);
      else
      {
        unsigned int const hunterIndex = walk-Hunter0;
        assert(hunterIndex<max_nr_hunter_walks);

        pos_in_cell[1] = '/';
        pos_in_cell = WriteWalkRtoL(pos_in_cell,huntertypes[hunterIndex].away);
      }

      if (is_piece_neutral(pos->spec[square]))
        pos_in_cell[0] = '=';
      else if (TSTFLAG(pos->spec[square],Black))
        pos_in_cell[0] = '-';
    }

    protocol_fprintf(stdout,"%s",cell);
  }
}

static void WriteBaseCells(position const *pos, square square_a)
{
  unsigned int column;
  square square;

  for (column = 0, square = square_a;
      column!=nr_files_on_board;
       ++column, square += dir_right)
  {
    piece_walk_type const walk = pos->board[square];

    char cell[fileWidth+1];
    char *pos_in_cell = cell + (sizeof cell)/2;

    snprintf(cell, sizeof cell, "%*c", fileWidth, ' ');

    if (CondFlag[gridchess] && !OptFlag[suppressgrid])
    {
      if (is_on_board(square+dir_down)
          && GridLegal(square,square+dir_down))
      {
        pos_in_cell[-1] = '-';
        pos_in_cell[0] = '-';
        pos_in_cell[+1] = '-';
      }
    }

    if (Hunter0<=walk && walk<Hunter0+max_nr_hunter_walks)
    {
      unsigned int const hunterIndex = walk-Hunter0;
      WriteWalkRtoL(pos_in_cell,huntertypes[hunterIndex].home);
    }

    protocol_fprintf(stdout,"%s",cell);
  }
}

static void WriteBorder(void)
{
  unsigned int column;
  char letter;

  assert(nr_files_on_board <= 'z'-'a');

  protocol_fprintf(stdout,"%s","+--");

  for (column = 0, letter = 'a'; column!=nr_files_on_board; ++column, ++letter)
  {
    char cell[fileWidth+1];
    snprintf(cell, sizeof cell, "-%c--", letter);
    protocol_fprintf(stdout,"%s",cell);
  }

  protocol_fprintf(stdout,"%s","-+\n");
}

static void WriteBlankLine(void)
{
  unsigned int column;

  protocol_fprintf(stdout,"%s","| ");
  protocol_fprintf(stdout,"%s"," ");

  for (column = 0; column!=nr_files_on_board; ++column)
    protocol_fprintf(stdout,"%s","    ");

  protocol_fprintf(stdout,"%s"," |\n");
}

void WriteBoard(position const *pos)
{
  unsigned int row;
  square square_a;

  assert(nr_rows_on_board<10);

  protocol_fputc('\n',stdout);
  WriteBorder();
  WriteBlankLine();

  for (row = 0, square_a = square_a8;
       row!=nr_rows_on_board;
       ++row, square_a += dir_down)
  {
    protocol_fprintf(stdout,"%d ",nr_rows_on_board-row);
    WriteRegularCells(pos,square_a);
    protocol_fprintf(stdout,"  %d", nr_rows_on_board-row);
    protocol_fputc('\n',stdout);

    protocol_fprintf(stdout,"%s","| ");
    WriteBaseCells(pos,square_a);
    protocol_fprintf(stdout,"%s","  |\n");
  }

  WriteBorder();
}

static void WriteMeta(void)
{
  protocol_fprintf(stdout,"%s","\n");
  protocol_fputs_c_multi(stdout,board_width,ActAuthor);
  protocol_fputs_c_multi(stdout,board_width,ActOrigin);
  protocol_fputs_c_multi(stdout,board_width,ActAward);
  protocol_fputs_c_multi(stdout,board_width,ActTitle);
}

static void WriteCondition(FILE* dummy, char const CondLine[], condition_rank rank)
{
  if (rank!=condition_end)
    protocol_fprintf_c(stdout,board_width,"%s\n",CondLine);
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
void output_plaintext_write_meta(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  WriteMeta();

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_board(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  WriteBoard(slices[si].u.position_handler.position);
  indentation = 0;

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_piece_counts(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  WritePieceCounts(slices[si].u.position_handler.position,indentation);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_royal_piece_positions(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  WriteRoyalPiecePositions(slices[si].u.position_handler.position);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_stipulation_options(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  indentation += WriteOptions(&being_solved);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_stipulation(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  indentation += WriteStipulation();

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_non_royal_attributes(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  WriteNonRoyalAttributedPieces(slices[si].u.position_handler.position);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_conditions(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  WriteConditions(0,&WriteCondition);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_mutually_exclusive_castlings(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  WriteCastlingMutuallyExclusive();

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_duplex(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  protocol_fprintf_c(stdout,board_width,"%s\n",OptTab[duplex]);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_halfduplex(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  protocol_fprintf_c(stdout,board_width,"%s\n",OptTab[halfduplex]);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_quodlibet(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  protocol_fprintf_c(stdout,board_width,"%s\n",OptTab[quodlibet]);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_grid(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  WriteGrid();

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_write_atob_intra(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  protocol_fputc('\n',stdout);
  protocol_fprintf_c(stdout,board_width,"=> (%s ->)\n",ColourTab[SLICE_STARTER(si)]);
  protocol_fputc('\n',stdout);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
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
void output_plaintext_end_of_position_writer_writers_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  protocol_fflush(stdout);

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static slice_index alloc_position_handler(slice_type type, position const *pos)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceFunctionParamListEnd();

  result = alloc_pipe(type);
  slices[result].u.position_handler.position = pos;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

void output_plaintext_position_writer_builder_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  assert(find_unique_goal(si).type!=goal_proofgame);
  assert(find_unique_goal(si).type!=goal_atob);

  {
    slice_index const prototypes[] = {
        alloc_pipe(STOutputPlainTextMetaWriter),
        alloc_position_handler(STOutputPlainTextBoardWriter,&being_solved),
        alloc_pipe(STOutputPlainTextStipulationWriter),
        alloc_pipe(STOutputPlainTextStipulationOptionsWriter),
        alloc_position_handler(STOutputPlainTextPieceCountsWriter,&being_solved),
        alloc_position_handler(STOutputPlainTextRoyalPiecePositionsWriter,&being_solved),
        alloc_position_handler(STOutputPlainTextNonRoyalAttributesWriter,&being_solved),
        alloc_pipe(STOutputPlainTextEndOfPositionWriters)
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_insertion_insert(si,prototypes,nr_prototypes);
  }

  pipe_solve_delegate(si);

  /* only build the position writers once per problem */
  pipe_remove(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

void output_plaintext_proof_position_writer_builder_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  {
    slice_index const prototypes[] = {
        alloc_pipe(STOutputPlainTextMetaWriter),
        alloc_pipe(STOutputPlainTextStartOfTargetWriter),
        alloc_position_handler(STOutputPlainTextBoardWriter,&proofgames_target_position),
        alloc_pipe(STOutputPlainTextStipulationWriter),
        alloc_pipe(STOutputPlainTextStipulationOptionsWriter),
        alloc_position_handler(STOutputPlainTextPieceCountsWriter,&proofgames_target_position),
        alloc_position_handler(STOutputPlainTextRoyalPiecePositionsWriter,&proofgames_target_position),
        alloc_position_handler(STOutputPlainTextNonRoyalAttributesWriter,&proofgames_target_position),
        alloc_pipe(STOutputPlainTextEndOfPositionWriters)
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_insertion_insert(si,prototypes,nr_prototypes);
  }

  pipe_solve_delegate(si);

  /* only build the position writers once per problem */
  pipe_remove(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

static slice_index alloc_atob_intra_writer(Side starter)
{
  slice_index result;

  TraceFunctionEntry(__func__);
  TraceEnumerator(Side,starter,"");
  TraceFunctionParamListEnd();

  result = alloc_pipe(STOutputPlainTextAToBIntraWriter);
  SLICE_STARTER(result) = starter;

  TraceFunctionExit(__func__);
  TraceFunctionResult("%u",result);
  TraceFunctionResultEnd();
  return result;
}

void output_plaintext_atob_start_position_writer_builder_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  assert(find_unique_goal(si).type==goal_atob);

  {
    slice_index const prototypes[] = {
        alloc_position_handler(STOutputPlainTextBoardWriter,&being_solved),
        alloc_position_handler(STOutputPlainTextPieceCountsWriter,&being_solved),
        alloc_position_handler(STOutputPlainTextRoyalPiecePositionsWriter,&being_solved),
        alloc_position_handler(STOutputPlainTextNonRoyalAttributesWriter,&being_solved),
        alloc_atob_intra_writer(SLICE_STARTER(si))
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_insertion_insert(si,prototypes,nr_prototypes);
  }

  pipe_solve_delegate(si);

  /* only build the position writers once per problem */
  pipe_remove(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

void output_plaintext_conditions_writer_builder_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  {
    slice_index const prototypes[] = {
        alloc_pipe(STOutputPlainTextConditionsWriter)
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_insertion_insert(si,prototypes,nr_prototypes);
  }

  pipe_solve_delegate(si);

  /* only build the position writers once per problem */
  pipe_remove(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

void output_plaintext_mutually_exclusive_castlings_writer_builder_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  {
    slice_index const prototypes[] = {
        alloc_pipe(STOutputPlainTextMutuallyExclusiveCastlingsWriter)
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_insertion_insert(si,prototypes,nr_prototypes);
  }

  pipe_solve_delegate(si);

  /* only build the position writers once per problem */
  pipe_remove(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

void output_plaintext_separate_duplex_writer_builder_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  {
    slice_index const prototypes[] = {
        alloc_pipe(STOutputPlainTextDuplexWriter)
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_insertion_insert(si,prototypes,nr_prototypes);
  }

  pipe_solve_delegate(si);

  /* only build the position writers once per problem */
  pipe_remove(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

void output_plaintext_separate_halfduplex_writer_builder_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  {
    slice_index const prototypes[] = {
        alloc_pipe(STOutputPlainTextHalfDuplexWriter)
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_insertion_insert(si,prototypes,nr_prototypes);
  }

  pipe_solve_delegate(si);

  /* only build the position writers once per problem */
  pipe_remove(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

void output_plaintext_separate_quodlibet_writer_builder_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  {
    slice_index const prototypes[] = {
        alloc_pipe(STOutputPlainTextQuodlibetWriter)
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_insertion_insert(si,prototypes,nr_prototypes);
  }

  pipe_solve_delegate(si);

  /* only build the position writers once per problem */
  pipe_remove(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

void output_plaintext_separate_grid_writer_builder_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  {
    slice_index const prototypes[] = {
        alloc_pipe(STOutputPlainTextGridWriter)
    };
    enum { nr_prototypes = sizeof prototypes / sizeof prototypes[0] };
    slice_insertion_insert(si,prototypes,nr_prototypes);
  }

  pipe_solve_delegate(si);

  /* only build the position writers once per problem */
  pipe_remove(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}

void output_plaintext_option_noboard_solve(slice_index si)
{
  TraceFunctionEntry(__func__);
  TraceFunctionParam("%u",si);
  TraceFunctionParamListEnd();

  while (SLICE_TYPE(SLICE_NEXT1(si))!=STStartOfSolvingMachinery)
    pipe_remove(SLICE_NEXT1(si));

  pipe_solve_delegate(si);

  TraceFunctionExit(__func__);
  TraceFunctionResultEnd();
}
