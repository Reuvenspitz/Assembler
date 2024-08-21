#ifndef MACRO_H
#define MACRO_H
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "scanner.h"

  struct Macro_unit{
    char* name;
    char* body;
    size_t size_of_body;
    struct  Macro_unit* next;
};
typedef struct Macro_unit  macro_unit;
macro_unit* build_new_macro(char *name,macro_unit *head);
void free_macro_table(macro_unit *mcr);
macro_unit* search_in_the_macro_table(char* name,macro_unit* head);
void add_line_to_macro(macro_unit *mcr,char *line);
BOOLEAN checks_whether_str_can_be_a_macro(char *str,macro_unit *head);






#endif
