#include "macro_handling.h"
/*this function controls deployment of the macros in the following way it opens a new file with the suffix .as and it reads line after line from the file ending.ax
 * If there is a macro definition it checks if the name is valid if so it saves it into a special linked list
 * If a macro name appears it writes all the macro body into the modified file ending in .am
 * Otherwise it copies the line into the modified file
 *(In accordance with the course instructions I assume that there are no errors in the macro definitions)
 *  The function returns the macro table for future use */
macro_unit* preprocessor(char* name,BOOLEAN *error_in_preprocessor) {
    FILE *modified = NULL;
    BOOLEAN   flag_is_macro = FALSE;
    scanner *source  = NULL;
    macro_unit *current = NULL;
    macro_unit *head = NULL;
    source = build_scanner(name, ".as");
    modified = open_file_to_write(name, ".am");


    read_line_from_file(source);


    while (!source->end_of_file) {

        if (!is_empty_line(source->current_line) && !is_a_comment(source->current_line)) { /* if a line is empty or is a comment (starts with ;) ignore it*/

            separate_token_from_line(source); /*read the first token from the line*/

            current = search_in_the_macro_table(source->current_token, head);/*search in the macro table if there is a macro of that name */
            if (current != NULL) {                   /* if current is not NULL there is a macro in that name   */
                fprintf(modified, "%s", current->body);   /*write macro content into the modified file   */
            } else if (strcmp(source->current_token, "mcr") == 0) { /*checks if the line contains a new macro definition */

                separate_token_from_line(source);   /* reads the name of the new macro from the line*/

                if (checks_whether_str_can_be_a_macro(source->current_token, head)) { /*checks that the name of the macro is valid (begins with a letter and contains only numbers and letters) and that there is no existing macro of that name and that the macro name is not reserved word  */

                    head = build_new_macro(source->current_token, head);/*add the new macro to the macro table */
                    flag_is_macro = TRUE;

                    while (flag_is_macro) {  /*read the macro body until endmcr appears */
                        read_line_from_file(source);
                        separate_token_from_line(source);
                        if (strcmp(source->current_token, "endmcr") == 0) { /*end of macro definition */
                            flag_is_macro = FALSE;
                            continue;

                        } else {
                            add_line_to_macro(head, source->current_line);/*add line to the macro body */
                        }

                    }

                } else {     /* if the name is invalid all the lines are ignored until endmcr appears */
                    printf("Invalid macro %s in line %d", source->current_token, source->line_number);
                    flag_is_macro = TRUE;
                    *error_in_preprocessor = TRUE;   /* Saves the error */
                    while (flag_is_macro) {            /*Ignores the lines until the end of the macro definition*/
                        read_line_from_file(source);
                        separate_token_from_line(source);
                        if (strcmp(source->current_token, "endmcr") == 0) {
                            flag_is_macro = FALSE;
                            continue;

                        }
                    }


                }


            } else{
                fprintf(modified,"%s\n",source->current_line); /*otherwise the line is added to the modified file */

            }
        }
        read_line_from_file(source);/*read the next line */
    }
    fclose(modified);
    *error_in_preprocessor = (*error_in_preprocessor) || source->error_in_file; /*If there is an error update a variable*/
    free_scanner(source);   /*free memory*/


    return head ; /*The function returns the macro table for future use*/

}

