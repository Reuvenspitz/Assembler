#ifndef SCANNER
#define SCANNER
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "instruction.h"


 /*This is a structure named scanner that assists
 * in reading line after line and token after token from every line from the file named "name" with the suffix "suffix"
 * and includes fields for storage of current data  and file status .
  * */
typedef struct {
    FILE *file;  /* points to the current file */
    char  *current_line,*current_token,*file_name;/*memory for storage of the next line and next token */
    size_t current_line_index,current_line_length,current_index_in_line,current_index_in_token; /*indicates the current length of the token in line and the current index of the token in line  */
    int line_number;      /* current line number*/
    BOOLEAN seen_newline;
    BOOLEAN seen_string;
    BOOLEAN end_of_file;   /* indicates if the file has ended */
    BOOLEAN end_of_line;   /* indicates if the line has ended */
    BOOLEAN error_in_file; /*indicates if there is an error in the file */
    BOOLEAN ignore_line;   /* indicates to ignore line and move to the next line*/
}scanner;


scanner* build_scanner(char *name,char *suffix);
void free_scanner (scanner *scn);
void read_line_from_file(scanner *scn);
void separate_token_from_line(scanner *scn);
BOOLEAN is_comment(const char *line);
void  skip_white_spaces_in_scanner(scanner *scn);







#endif
