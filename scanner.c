#include "scanner.h"


/*this function receives a name and suffix and creates a  scanner to the file named "name" with the suffix "suffix"
 * and initializes all the fields to their starting value .
 * */

scanner *build_scanner(char *name, char *suffix) {

    scanner *pointer;

    pointer = allocate_memory(sizeof(scanner));  /*allocates memory to the scanner */
    pointer->current_line = allocate_memory(MAX_LINE_SIZE + 1); /*allocates memory for storage of the next line */
    pointer->current_token = allocate_memory(MAX_LINE_SIZE);/*allocates memory for storage of the next token from the current line */
    pointer->file = open_file_to_read(name, suffix); /* points to the current file */
    pointer->end_of_file = FALSE;
    pointer->error_in_file = FALSE;   /*initializes all the fields to their starting value. */
    pointer->line_number = 0;
    pointer->file_name = (char *) allocate_memory((int) (strlen(name) + strlen(suffix) + 1));

    strcat(pointer->file_name, name);


    return pointer;
}
/*
 * this function receives a pointer to scanner and frees the memory that was allocated to it
 * */

void free_scanner(scanner *scn) {
    if (scn == NULL) {
        printf("scanner is null \n");
        exit(EXIT_FAILURE);
    }
    if (fclose(scn->file) == EOF) {
        perror("can't close file");
        exit(EXIT_FAILURE);
    }
    free_memory(scn->current_line);
    free_memory(scn->current_token);
    free_memory(scn->file_name);
    free_memory(scn);
}
/*this function receives pointer to scanner and skips white spaces from the current index in line and onwards
 * */
void skip_white_spaces_in_scanner(scanner *scn) {
    while (isspace(scn->current_line[scn->current_index_in_line]) && /*checks if the current character is a white space and that we haven't reached the end of the line */
           scn->current_index_in_line < scn->current_line_length)
        scn->current_index_in_line++;

}
/*this function reads the next line from the file, saves it in the field current_line saves it's length,
 * checks that there are no more than 80 characters in the line. If there are more than 80 it prints an error warning
 * and saves that there is an error in the variable error_in_file and ignores this line
 * In addition it checks if we have reached the end of the file if so it updates the variable_end_of file
 */

void read_line_from_file(scanner *scn) {
    int a;
    scn->end_of_line = FALSE;
    scn->seen_string = FALSE; /*initializes the starting values */
    memset(scn->current_line, 0, MAX_LINE_SIZE + 1); /*resets the memory */

    if (fgets(scn->current_line, MAX_LINE_SIZE + 1, scn->file) == NULL) { /*reads the next line and checks if end of file */
        scn->end_of_file = TRUE;             /*if so updates end_of_file  */
    }
    scn->current_line_length = strlen(scn->current_line); /*saves the length of the line */

    if (scn->current_line_length >= MAX_LINE_SIZE) {  /*checks that the line is no longer than 80 characters */
        printf("Line number %d is longer than 80 characters \n", scn->line_number + 1);    /*if so notifies the error */
        scn->error_in_file = TRUE;
        while ((a = fgetc(scn->file)) != '\n' && a != EOF) {
        }
        scn->current_line[0] = 0;              /*skips the rest of the line */
        return;
    }

    scn->current_index_in_line = 0;

    while (scn->current_line_length && isspace(scn->current_line[scn->current_line_length - 1])) {

        scn->current_line[scn->current_line_length - 1] = 0;  /*erases white spaces from the right side of the line */
        scn->current_line_length--;     /*updates length of line */
    }

    scn->line_number++;  /*updates number of lines read */
}
/*this function receives pointer to scanner and reads the next token from the line
 * the end of the token is indicated by a white space or a comma or an opening(appended to the previous token) or closing bracket or an equal sign
 *or by reading  comma, equal sign, or closing bracket
 * if there is a string enclosed by quotation marks it is read from the first mark till the last mark in the linn
 * */
void separate_token_from_line(scanner *scn) {
    size_t i, j;
    char current;
    memset(scn->current_token, 0, MAX_LINE_SIZE); /*resets the memory */
    scn->current_index_in_token = 0; /*initializes the starting values */
    skip_white_spaces_in_scanner(scn);/*skip white spaces*/

    if (scn->current_index_in_line == scn->current_line_length) {  /*checks if we reached the end of the line */
        scn->end_of_line = TRUE;        /*if so updates the variable end_of_line and returns */
        return;
    }

    while (scn->current_index_in_line != scn->current_line_length) {
        current = scn->current_line[scn->current_index_in_line]; /*reads the next character */
        if (isspace(current)) {     /*checks that it isn't a white space */
            break;
        }
        if (current == '[') {      /*checks if the character is an opening bracket*/
            scn->current_token[scn->current_index_in_token] = current;
            scn->current_index_in_line++;
            scn->current_index_in_token++;  /*if so includes the character in the token and returns */
            return;
        }
        if (current == '"' && scn->current_index_in_token == 0) {    /*checks if it is the beginning of a string */

            for (i = scn->current_line_length - 1; i > scn->current_index_in_line; i--) {
                if (scn->current_line[i] == '"')
                    break;                        /*if so find the last quotation marks in the line*/
            }
            for (j = scn->current_index_in_line; j <= i; ++j) {
                scn->current_token[scn->current_index_in_token] = scn->current_line[j];
                scn->current_index_in_line++;
                scn->current_index_in_token++;  /*copies what is between the quotation marks and returns*/
            }
            return;
        }
        if (current == ',' || current == ']' || current == '=') {  /*checks if the character is  ','or ']' or '=' */
            if (scn->current_index_in_token != 0) {    /*checks if we have already read characters, if so stops*/
                break;
            }

            scn->current_token[0] = current;   /*if not copies it and returns */
            scn->current_index_in_token = 1;
            scn->current_index_in_line++;
            break;
        }

        scn->current_token[scn->current_index_in_token] = current; /*copies the char, updates the variable and  continues the loop*/
        scn->current_index_in_line++;
        scn->current_index_in_token++;
    }
}







