#if !defined(OUTPUT_LATEX_TREE_H)
#define OUTPUT_LATEX_TREE_H

#include "stipulation/stipulation.h"
#include "utilities/boolean.h"

#include <stdio.h>

/* Instrument the stipulation structure with slices that implement
 * plaintext tree mode output.
 * @param si identifies slice where to start
 * @param is_setplay is si part of set play?
 */
void solving_insert_output_latex_tree_slices(slice_index si,
                                             boolean is_setplay,
                                             FILE *file);

/* Write a possibly pending move decoration
 */
void output_latex_tree_write_pending_move_decoration(void);

#endif
