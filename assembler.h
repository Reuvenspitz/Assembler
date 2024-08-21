

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "macro_handling.h"
#include "util.h"
#include "parser.h"

parser * first_pass(char *name,macro_unit *macro_table);
BOOLEAN check_and_print_ent_file(parser *prs);
void print_ob_file(parser *prs);
void print_ext_file(parser *prs);


BOOLEAN assembler(char *name);




#endif
