#ifndef MMN_14_PARSER_H
#define MMN_14_PARSER_H
#include "scanner.h"
#include "util.h"
#include "instruction.h"
#include <stdarg.h>
#include "macro.h"

/*
 * This enum represent every possible state in the parser
 *
 * */
enum State{END,
        GET_FIRST_TOKEN,
        GET_FIRST_ARGUMENT,
        GET_SECOND_ARGUMENT,
        GET_ONE_COMMA,
        GET_CLOSING_BRACKET,
        GET_OFFSET,
        GET_EQUAL_FOR_DEFINE,
        GET_DEFINE_NAME,
        GET_DEFINE_VALUE,
        GET_PARAMETER_FOR_DATA,
        GET_COMMA_FOR_DATA,
        GET_ARGUMENT_FOR_STRING,
        GET_ARGUMENT_FOR_ENTRY,
        GET_ARGUMENT_FOR_EXTERN
};
typedef enum State state;
#define FIRST 1
#define SECOND 2


/*This is a structure named parser that contains all the information needed to parse a file
 * it contains a scanner pointer, a boolean that indicates if the current line is a label, a boolean that indicates if the current line should be ignored
 * an integer that represents the instruction counter, an integer that represents the data counter, an integer that represents the current argument
 * a string that represents the label name, a macro_unit pointer that represents the macro table, a state that represents the current state of the parser
 * an entry_list pointer that represents the entry list, a data_image pointer that represents the current data, a data_image pointer that represents the head of the data list
 *  a label_to_replace pointer that represents the labels to replace list, an instruction pointer that represents the current instruction, an instruction pointer that represents the head of the instruction list
 *   a command_with_info that represents the current command, a symbol_unit pointer that represents the head of the symbol list, a symbol_unit pointer that represents a symbol unit
 *
 * */
struct Parser{
    scanner *scn;
    BOOLEAN is_label;
    BOOLEAN ignore_line;
    int ic;
    int dc;
    int current_arg;
    char *label_name;
    macro_unit *macro_table;

    state q;
    entry_list *entry_List;
    data_image *current_data;
    data_image *head_of_data_list;
    label_to_replace *labels_to_replace_list;
    instruction *current_instruction;
    instruction *head_of_instruction_list;
    command_with_info current_command;
    symbol_unit *head_of_symbol_list;
};
typedef struct Parser parser;

void free_parser(parser *ptr);
parser * build_parser(char *name,char *suffix,macro_unit *macr0_table);
void next_line_in_parser(parser *ptr);
void parse_token(parser *prs);

BOOLEAN replace_labels(parser *prs);
void update_labels(parser *prs);
void add_data_to_the_list(parser *prs,short value);







#endif
