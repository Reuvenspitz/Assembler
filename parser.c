#include "parser.h"
void get_first_token(parser *prs);
void get_define_name(parser *prs);
void get_equal_for_define(parser *prs);
void get_define_value(parser *prs);
void end(parser *prs);
void get_argument(parser *prs);
void get_first_argument(parser *prs);
void get_second_argument(parser *prs);
void get_offset(parser *prs);
void get_closing_bracket(parser *prs);
void get_one_comma(parser *prs);
void get_parameter_for_data(parser *prs);
void get_comma_for_data(parser *prs);
void get_argument_for_string(parser *prs);
void get_argument_for_entry(parser *prs);
void get_argument_for_extern(parser *prs);
BOOLEAN find_define_in_symbol_table(symbol_unit *head,long *result,char *name);
BOOLEAN checks_if_string_is_valid_label(parser *prs,char *string);
/* =========================================================================================*/

/*This function accepts parser and the message that can be composed of several arguments
 * prints an error notification with the file name and the line number of the error and the arguments that were passed
 * and updates the parser variables to ignore that line and that there is an error in the file
 * */
void error(parser *ptr, const char *message, ...) {
    va_list ap;
    va_start(ap, message);
    printf("Error in file %s at line %d: ", ptr->scn->file_name, ptr->scn->line_number);
    vprintf(message, ap);
    va_end(ap);
    printf("\n");      /*prints the message */
    ptr->ignore_line = TRUE;
    ptr->is_label = FALSE;
    ptr->scn->error_in_file = TRUE; /*updates the parser variables */
}
/*
 * this function reads the next line from the file and initializes all the parser variables (and reset the label name field)
 * to the default values
 * and assign the state GET_FIRST_TOKEN to the variable q
 * */

void next_line_in_parser(parser *prs) {

    read_line_from_file(prs->scn); /* red the new line*/
    memset(prs->label_name, 0, MAX_LABEL_SIZE);/*reset the label name*/

    prs->q = GET_FIRST_TOKEN; /*assign the state GET_FIRST_TOKEN to the variable q*/
    prs->is_label = FALSE;
    prs->ignore_line = FALSE;
}
/*this function accepts parser and analyses the next token according to the state q
 * by calling the function that is appropriate to the state
 * */
void parse_token(parser *prs) {

    switch (prs->q) {    /* the switch loop reviews all possible states and calls the appropriate function */

        case END:
            end(prs);
            return;

        case GET_FIRST_TOKEN:
            get_first_token(prs);
            return;
        case GET_FIRST_ARGUMENT:
            get_first_argument(prs);
            return;

        case GET_SECOND_ARGUMENT:
            get_second_argument(prs);
            return;

        case GET_OFFSET:
            get_offset(prs);
            return;

        case GET_CLOSING_BRACKET:
            get_closing_bracket(prs);
            return;

        case GET_ONE_COMMA:
            get_one_comma(prs);
            return;

        case GET_DEFINE_NAME:
            get_define_name(prs);
            return;

        case GET_EQUAL_FOR_DEFINE:
            get_equal_for_define(prs);
            return;

        case GET_DEFINE_VALUE:
            get_define_value(prs);
            return;

        case GET_PARAMETER_FOR_DATA:
            get_parameter_for_data(prs);
            return;

        case GET_COMMA_FOR_DATA:
            get_comma_for_data(prs);
            return;

        case GET_ARGUMENT_FOR_STRING:
            get_argument_for_string(prs);
            return;
        case GET_ARGUMENT_FOR_ENTRY:
            get_argument_for_entry(prs);
            return;
        case GET_ARGUMENT_FOR_EXTERN:
            get_argument_for_extern(prs);
            return;
    }


}
/*
 * this function deals with the state in which we have to receive an argument for .extern
 * it checks that the argument is valid for the command if so it saves it in the symbol table
 * if not it prints an error notification
 * (in accordance with the instructions in the course forum we assumed that there is only one argument after extern)
 *
 * */

void get_argument_for_extern(parser *prs) {

    char *token = prs->scn->current_token;
    if(is_empty_line(token)){  /* checks that the token isn't empty*/
        error(prs,"missing argument\n");/*is so it prints error and return */
        return;
    }

    if (!checks_if_string_is_valid_label(prs, token)) {  /*calls to a function to check if the label is valid if not the function prints error  */
        return;                                                  /*if it's not valid return*/
    }
    prs->head_of_symbol_list = add_symbol(token, prs->head_of_symbol_list);
    set_symbol_type(prs->head_of_symbol_list, EXTERN);
    set_symbol_value(prs->head_of_symbol_list, 0);
    prs->q = END;                                                       /*adds the label to the symbol table with the type extern and moves q to the end of line state*/
}

/*
 * this function deals with the state in which we have to receive an argument for .entry
 * it checks that the argument is valid for the command if so it saves it in the entry list
 * if not it prints an error notification
 * (in accordance with the instructions in the course forum we assumed that there is only one argument after entry)
 *
 * */

void get_argument_for_entry(parser *prs) {
    char *token = prs->scn->current_token;

    if(is_empty_line(token)){                /* checks that the token isn't empty*/
        error(prs,"missing argument\n"); /*is so it prints error and return*/
        return;
    }

    if (!checks_whether_str_can_be_a_label(token)) {            /*checks if the token can syntactically be a label*/
        error(prs, "The string  %s is not valid to be a label \n",token);  /*if not print error and return */
        return;
    }
    prs->entry_List = add_entry(prs->entry_List, prs->scn->current_token, prs->scn->line_number);/*adds the label to the entry list */
    prs->q = END;     /*moves q to the end of line state */
}
/* this function deals with the state in which we have to receive a string of characters enclosed by quotation marks
 * (in accordance with instructions in the course forum  reads the string from the opening quotation marks until the final quotation marks in the line )
 * checks if all the characters in the string are printable if so adds them to the data list adds zero at the end of the string
 * if there was a label definition at the beginning of the line it adds it to the symbol table
 *
 *
 * */
void get_argument_for_string(parser *prs) {
    char *string;
    size_t i;
    string = prs->scn->current_token;
    if(is_empty_line(string)){                  /* checks that the token isn't empty*/
        error(prs,"missing argument\n");/*is so it prints error and return */
        return;
    }
    if (prs->is_label) {                                                      /*if there is a label definition it calls to a function to check if the label is valid if not the function prints error  */
        if (!checks_if_string_is_valid_label(prs, prs->label_name)) {
            return;
        }
        prs->head_of_symbol_list = add_symbol(prs->label_name, prs->head_of_symbol_list);/*if it's valid it adds the label to the symbol table with the current dc and the type string */
        set_symbol_value(prs->head_of_symbol_list, prs->dc);
        set_symbol_type(prs->head_of_symbol_list, STRING);
    }
    for (i = 1; i < strlen(string) - 1; i++) {         /*goes over  character by character in the loop and checks if the character is printable*/

        if (!isprint(string[i]) /*implementation*/) {

            error(prs, "couldn't store the char %c\n", string[i]); /*if not prints error and returns*/
            return;
        }
        add_data_to_the_list(prs, string[i]);
        prs->dc++;                                      /*if it's valid adds it to the data list and updates the dc*/

    }
    add_data_to_the_list(prs, 0); /*adds 0 at the end of the string*/
    prs->dc++;
    prs->q = END;                               /*moves q to the end of line state */
}
/*this function deals with the state when we need a separating comma between arguments of data
 * */

void get_comma_for_data(parser *prs) {
    if (*prs->scn->current_token == ',') { /*if so it moves q to the state GET_PARAMETER_FOR_DATA*/
        prs->q = GET_PARAMETER_FOR_DATA;
        return;
    }
    error(prs, "missing comma \n"); /* if not prints error*/
}

/*this function deals with the state when we need to get the next argument for the command data
 * checks if the arguments are valid if not prints error if valid adds it to data list
 * if it is the first argument and there is a label definition in the beginning of the line it adds it to the symbol table with type data
 * checks if there are more characters in the rest of the line if so moves q to the state of get comma if not moves q to the end of line state
 * */


void get_parameter_for_data(parser *prs) {
    char *token;
    long par;
    token = prs->scn->current_token;
    if(is_empty_line(token)){                   /* checks that the token isn't empty*/
        error(prs,"missing parameter\n");   /*is so it prints error and return */
        return;
    }
    if (converts_a_string_to_a_number(token, &par) ||
        find_define_in_symbol_table(prs->head_of_symbol_list, &par, token)) { /*checks if the token is a number or a symbol that was already defined in the type define */

        if (par > MAX_12_BITS_NUMBER || par < MIN_12_BITS_NUMBER) {        /*checks that the number can be written in 14 bits */
            error(prs, "The number are not  \n"); /*if not prints error and returns */
            return;
        }
        if (prs->is_label) {
            if (!checks_if_string_is_valid_label(prs, prs->label_name)) { /*if there is a label definition it calls to a function to check if the label is valid if not the function prints error  */

                return;
            }
            prs->head_of_symbol_list = add_symbol(prs->label_name, prs->head_of_symbol_list); /*if it's valid it adds the label to the symbol table with the current dc and the type data */
            set_symbol_value(prs->head_of_symbol_list, prs->dc);
            set_symbol_type(prs->head_of_symbol_list, DATA);
            prs->is_label = FALSE;  /*updates the is_label variable */
        }
        add_data_to_the_list(prs, (short) par);/*adds the number to the data list */
        prs->dc++;                                      /*updates the dc */
        if (prs->scn->current_line_length == prs->scn->current_index_in_line) { /*checks if there are more characters in the rest of the line */
            prs->q = END;     /*if not moves q to the end of line state */
            return;
        }
        prs->q = GET_COMMA_FOR_DATA;/*if so moves q to the state of get comma */
        return;

    }
    error(prs, "The string %s is not an integer or a define \n", token);/*if the token is not a number or a define prints error */

}
/*this function deals with the state end and checks if there are more characters in the rest of the line
 * if so prints an error notification
 * if not updates the end of line variable to true
 * */
void end(parser *prs) {
    if (*(prs->scn->current_token) == 0) {/*checks if there are more characters in the rest of the line */
        prs->scn->end_of_line = TRUE;
    } else {
        error(prs, "Unnecessary text at the end of a line");/*if so prints an error notification */

    }
}
/*this function deals with the state in which we have to receive a name for a define
 * it calls to a function to  check that the name if not that function prints an error notification
 * if the name is valid it adds it to the symbol table with the type define and moves q to the state of get equal for define
 *
 * */

void get_define_name(parser *prs) {

    char *token = prs->scn->current_token;
    if(is_empty_line(token)){         /* checks that the token isn't empty*/
        error(prs,"missing define name\n");/*is so it prints error and return */
        return;
    }
    if (checks_if_string_is_valid_label(prs, token)) {/*calls to a function to check if the label is valid if not the function prints error  */
        prs->head_of_symbol_list = add_symbol(token, prs->head_of_symbol_list);
        set_symbol_type(prs->head_of_symbol_list, DEFINE);                          /*if it's valid it adds the label to the symbol table with the type define */
        prs->q = GET_EQUAL_FOR_DEFINE;
        return;
    }

}
/*this function deals with the state in which we have to receive an equal sign for a define
 * if the equal sign is not present it prints an error notification
 * if the equal sign is present it moves q to the state of get define value
 * */

void get_equal_for_define(parser *prs) {
    if (*(prs->scn->current_token) == '=') {/*checks if the equal sign is present */
        prs->q = GET_DEFINE_VALUE;
        return;
    } else
        error(prs, " When defining a constant, the equal sign must come after the name\n");/*if the equal sign is not present it prints an error notification */

}
/*this function deals with the state in which we have to receive a value for a define
 * it checks if the value is valid if not prints an error notification
 * if the value is valid it adds it to the symbol table and moves q to the end of line state
 * */

void get_define_value(parser *prs) {
    long value;
    if(is_empty_line(prs->scn->current_token)){ /* checks that the token isn't empty*/
        error(prs,"missing define value\n"); /*is so it prints error and return */
        return;
    }

    if (converts_a_string_to_a_number(prs->scn->current_token, &value)) { /*checks if the token is a number */
        if (value < MAX_DEFINE_NUMBER && value > MIN_DEFINE_NUMBER) {                /*checks if the number is in the range of 14 bits */
            set_symbol_value(prs->head_of_symbol_list, (int) value);   /*if so it adds the number to the symbol table */
            prs->q = END;
            return;
        }
        error(prs, "The number is outside of the permitted range\n");   /* if the number is not in the range of 14 bits it prints an error notification */

    }
}
/* this function deals with the state in which we have to receive the first argument for a command
 * it checks if the argument is valid for the command if so it saves it in the instruction list and moves q to the appropriate state
 * if not it prints an error notification
 *
 * */

void get_first_argument(parser *prs) {
    if (is_empty_line(prs->scn->current_token)){ /* checks that the token isn't empty*/
        error(prs,"missing argument\n");  /*is so it prints error and return */
        return;
    }
    get_argument(prs); /*calls to a function to get the argument */
    if ((prs->current_command.num_of_operands == MAX_OP && prs->current_instruction->source_mode == INDEX_METHOD) ||
        (prs->current_command.num_of_operands == FIRST && prs->current_instruction->dest_mode == INDEX_METHOD)) {    /*checks if the operand requires an offset */
        prs->q = GET_OFFSET;                                                                                            /*if so moves q to the state of get offset and return */
        return;
    }
    if (prs->current_command.num_of_operands == MAX_OP) {   /*checks if the command has two operands */
        prs->current_arg = SECOND;                      /*if so updates the current argument to the second argument */
        prs->q = GET_ONE_COMMA;                          /*moves q to the state of get one comma  and return*/
        return;
    }
    prs->q = END;                                   /*if the command has only one operand moves q to the end of line state */
}
/*this function deals with the state in which we have to receive the second argument for a command
 * it checks if the argument is valid for the command if so it saves it in the instruction list and moves q to the appropriate state
 * if not it prints an error notification
 *
 * */

void get_second_argument(parser *prs) {
    if(is_empty_line(prs->scn->current_token)){ /* checks that the token isn't empty*/
        error(prs,"missing second argument\n");  /*is so it prints error and return */
        return;
    }

    get_argument(prs);                                      /*calls to a function to get the argument */
    if (prs->current_instruction->dest_mode == INDEX_METHOD) { /*checks if the operand requires an offset */
        prs->q = GET_OFFSET;                                    /*if so moves q to the state of get offset and return */
        return;
    }
    prs->q = END;                                       /*if the operand doesn't require more information it moves q to end of line */
}
/*this function deals with the state in which we have to receive a comma between the first and the second argument for a command
 * if the comma is not present it prints an error notification
 * if the comma is present it moves q to the state of get second argument
 *
 * */

void get_one_comma(parser *prs) {
    if (*prs->scn->current_token == ',') {  /*checks if the comma is present */
        prs->q = GET_SECOND_ARGUMENT;       /*if so moves q to the state of get second argument */
        prs->current_arg = SECOND;          /*updates the current argument to the second argument */
        return;
    }
    error(prs, "A comma must follow the first argument \n");  /*if the comma is not present it prints an error notification */
}
/*this function deals with the state in which we have to receive an offset for an index method operand
 *it checks if the offset is valid if so it saves it in the instruction list and moves q to the state of get closing bracket
 * if not it prints an error notification and returns
 *
 * */

void get_offset(parser *prs) {
    char *token;
    int *offset;
    long value;
    symbol_unit *symbol;
    token = prs->scn->current_token;
    if (is_empty_line(token)){                          /* checks that the token isn't empty*/
        error(prs,"missing offset\n");
        return;
    }
    if (prs->current_arg == FIRST && prs->current_instruction->num_of_operands == MAX_OP) { /*checks if the current argument is for source or for dest*/
        offset = &prs->current_instruction->source.address_with_index.offset;              /*if it's for source updates the offset to the source offset */
    } else {
        offset = &prs->current_instruction->dest.address_with_index.offset;                 /*if it's for dest updates the offset to the dest offset */
    }

    if (converts_a_string_to_a_number(token, &value)) {           /*checks if the token is a number */
        if (value < MAX_DEFINE_NUMBER && value > MIN_DEFINE_NUMBER) {       /*checks if the number is in the range of 14 bits */
            *offset = (int) value;                               /*if so it adds the number to the instruction list */
            prs->q = GET_CLOSING_BRACKET;             /*moves q to the state of get closing bracket and return*/
            return;

        }
        error(prs, "The value is illegal");   /*if the number is not in the range of 14 bits it prints an error notification and return */
        return;
    }
    symbol = search_in_the_symbol_table(token, prs->head_of_symbol_list);
    if (symbol != NULL && symbol->type == DEFINE) {                 /*checks if the token is a define  that already define*/
        *offset = symbol->value;           /*if so it adds the value of the define to the instruction list */
        prs->q = GET_CLOSING_BRACKET;    /*moves q to the state of get closing bracket and return*/
        return;
    }
    error(prs, "The offset %s must be number or define \n ",token); /*if the token is not a number or a define it prints an error notification and return */
}
/*this function deals with the state in which we have to receive a closing bracket for an index method operand
 * if the closing bracket is not present it prints an error notification
 * if the closing bracket is present it moves q to the appropriate state
 * */

void get_closing_bracket(parser *prs) {

    if (*prs->scn->current_token == ']') {      /*checks if the closing bracket is present */
        if (prs->current_arg == FIRST && prs->current_command.num_of_operands == MAX_OP) {/*checks if we are expecting another argument*/
            prs->q = GET_ONE_COMMA;                                                 /*if so moves q to the state of get one comma */
            return;
        }
        prs->q = END;     /*if we are not expecting another argument moves q to the end of line state */
        return;

    }

    error(prs, "A closing bracket must follow an offset \n");  /*if the closing bracket is not present it prints an error notification */
}
/*this function deals with the state in which we have to receive arguments for a command
 * it checks if the arguments required are dest or source and checks which operand is the current token
 * it checks if the operand matches the command if so it updates the instruction list if not it prints an error notification
 *
 * */

void get_argument(parser *prs) {
    char *token;
    long value;
    Reg reg;
    Addressing_method *adr;
    BOOLEAN *modes;         /*pointer to an array that represents the addressing methods permitted in the current command  */
    operand *current_operand; /*pointer to the current operand */

    token = prs->scn->current_token;
    if (prs->current_arg == FIRST && prs->current_instruction->num_of_operands == MAX_OP) {  /*checks if the current argument is for source or for dest and updates the pointers in accordance*/
        current_operand = &prs->current_instruction->source;
        adr = &prs->current_instruction->source_mode;
        modes = prs->current_command.source_modes;
    } else {
        current_operand = &prs->current_instruction->dest;
        modes = prs->current_command.dest_modes;
        adr = &prs->current_instruction->dest_mode;

    }


    if (*token == '#') {      /*checks if the operand is an immediate operand */
        token++;   /*if so it deletes the character # */
        prs->ic++;              /*updates the ic */
        if (modes[IMMEDIATE_METHOD]) {   /*checks if the command permits an immediate operand */
            if (converts_a_string_to_a_number(token, &value)) {    /*checks if the token is a number */

                if (value < MIN_12_BITS_NUMBER || value > MAX_12_BITS_NUMBER) {  /*checks if the number is in the range of 12 bits */
                    error(prs, "The number %d is not in the range of 12 bits\n", value); /*if not prints an error notification and return */

                    return;
                }
                current_operand->immediate = (int) value; /*if the number is in the range of 12 bits it adds the number to the instruction list */
                *adr = IMMEDIATE_METHOD;  /*updates the addressing method */
                return;
            }

            if (find_define_in_symbol_table(prs->head_of_symbol_list, &value, token)) {/*checks if the token is a define that was already defined */

                if (value < MIN_12_BITS_NUMBER || value > MAX_12_BITS_NUMBER) {  /*checks if the number is in the range of 12 bits */
                    error(prs, "The number %d is not in the range of 12 bits\n", value); /*if not prints an error notification and return */

                    return;
                }
                current_operand->immediate = (int) value;
                return;
            }
            error(prs, "A operand that starts with # must be a number or a define\n"); /*if the token is not a number or a define it prints an error notification and return */
            return;
        }
        error(prs, "Operand cannot be an immediate");/*if the command doesn't permit an immediate operand it prints an error notification and return */
        return;
    }


    if ((reg = find_register(token)) != ERROR_REG) { /*checks if the operand is a register */

        if (modes[REG_METHOD]) {   /*checks if the command permits a register operand */
            current_operand->reg = reg;  /*if so it adds the register to the instruction list */
            *adr = REG_METHOD;          /*updates the addressing method */
            if (prs->current_arg == SECOND && prs->current_instruction->source_mode == REG_METHOD) {/*checks if the ic needs to be updated*/
                return;
            }
            prs->ic++;
            return;
        }

    }
    if (is_a_index_method(token)) { /*checks if the operand is an index method operand */

        if (modes[INDEX_METHOD]) { /*checks if the command permits an index method operand */

            token[strlen(token) - 1] = 0;/*deletes the character [*/
            if (!checks_whether_str_can_be_a_label(token)) {      /*checks if the token can syntactically be a label */
                error(prs, "The token %s cannot be a label \n", token); /*if not prints an error notification and return */
                return;
            }

            current_operand->address_with_index.address = UNKNOWN_VALUE;
            current_operand->address_with_index.is_ext = FALSE;

            prs->labels_to_replace_list = add_label_to_replace(token, current_operand, prs->labels_to_replace_list,/* adds the label to the labels to replace list */
                                                               prs->ic, prs->scn->line_number, INDEX_METHOD);
            *adr = INDEX_METHOD; /*updates the addressing method */
            prs->ic = prs->ic + SECOND;/*updates the ic */
            return;
        }
        error(prs, "Incompatible operand"); /*if the command doesn't permit an index method operand it prints an error notification and return */
        return;
    }
    /*the last option is an address method*/
    if (!checks_whether_str_can_be_a_label(token)) {      /*checks if the token can syntactically be a label */
        error(prs, "The token %s cannot be a label \n", token); /*if not prints an error notification and return */
        return;
    }
    current_operand->regular_address.address = UNKNOWN_VALUE;
    current_operand->regular_address.is_ext = FALSE;


    prs->labels_to_replace_list = add_label_to_replace(token, current_operand, prs->labels_to_replace_list, prs->ic,/* adds the label to the labels to replace list*/
                                                       prs->scn->line_number, DIRECT_METHOD);
    *adr = DIRECT_METHOD; /*updates the addressing method */
    prs->ic++;   /*updates the ic */
}
/*this function checks if a string is a define that was already defined
 * */


BOOLEAN find_define_in_symbol_table(symbol_unit *head, long *result, char *name) {
    symbol_unit *s;
    s = search_in_the_symbol_table(name, head); /*searches for the label in the symbol table */
    if (s != NULL && s->type == DEFINE) {    /*checks if the label is a define  type*/
        *result = s->value;           /*if so it updates the result to the value of the define  and return true*/
        return TRUE;
    }
    return FALSE; /*otherwise it returns false */

}
/*this function deals with the state in which we have to receive the first token in the line
 * it checks if the token is a label definition if so it saves it in the label name field and updates the is label variable
 * and q state  stays in get first  token
 * else it checks what command the token is and moves q to the appropriate state
 * */


void get_first_token(parser *prs) {
    directive d;
    char *token = prs->scn->current_token;

    if (is_empty_line(token)){    /* checks that the token isn't empty*/
        error(prs,"missing instruction/action\n");
        return;
    }

    d = find_statement_type(token);/*checks what command the token is */


    if (is_a_label(token)) { /*checks if the token is a label definition */
        if (prs->is_label) {    /*checks if there is already a label definition */
            error(prs, "Defining  label after  label  is invalid \n"); /*if so prints an error notification and return */
            return;
        }

        prs->is_label = TRUE;        /*updates the  label variable */
        memset(prs->label_name, 0, MAX_LABEL_SIZE + 1);
        token[strlen(token) - 1] = 0;
        strcat(prs->label_name, token);
        return;

    }


    switch (d) { /*the switch loop reviews all possible commands and moves q to the appropriate state */

        case ERROR_STATE:
            error(prs, "Unknown command");
            return;

        case DATA:

            prs->q = GET_PARAMETER_FOR_DATA;
            return;

        case STRING:

            prs->q = GET_ARGUMENT_FOR_STRING;
            return;

        case ENTRY:
            if (prs->is_label) {
                printf("Warning- there is a label definition before an entry command\n");
            }
            prs->q = GET_ARGUMENT_FOR_ENTRY;
            return;

        case EXTERN:
            if (prs->is_label) {
                printf("Warning- there is a label definition before an extern command \n");
            }
            prs->q = GET_ARGUMENT_FOR_EXTERN;
            return;

        case DEFINE:
            if (prs->is_label) {
                error(prs, "A label cannot come before a define \n");
                return;
            }

            prs->q = GET_DEFINE_NAME;
            return;

        case CODE:
            prs->current_command = find_command(token); /*finds the command that the token represents */

            if (prs->current_instruction == NULL) { /*checks if it's the first command in the file */

                prs->current_instruction = add_instruction(prs->current_command.opcode, prs->ic,
                                                           prs->current_command.num_of_operands);
                prs->head_of_instruction_list = prs->current_instruction;         /*if so it adds the command to the heda of the instruction list */
            } else {

                prs->current_instruction->next = add_instruction(prs->current_command.opcode, prs->ic,
                                                                 prs->current_command.num_of_operands);
                prs->current_instruction = prs->current_instruction->next;               /*if it's not the first command it adds the command to the instruction list */
            }
            if (prs->is_label) {         /*checks if there is a label definition */
                if (!checks_if_string_is_valid_label(prs, prs->label_name)) { /*if so it calls to a function to check if the label is valid if not the function prints error  */

                    return;
                }
                prs->head_of_symbol_list = add_symbol(prs->label_name, prs->head_of_symbol_list); /*if it's valid it adds the label to the symbol table with the current ic and the type code */
                set_symbol_type(prs->head_of_symbol_list, CODE);
                set_symbol_value(prs->head_of_symbol_list, prs->ic);
            }
            prs->ic++; /*updates the ic */

            if (prs->current_command.num_of_operands == 0) { /*checks if the command has no operands */

                prs->q = END; /*if so moves q to the end of line state */
                return;
            }
            prs->current_arg = FIRST; /*updates the current argument to the first argument */
            prs->q = GET_FIRST_ARGUMENT;/*moves q to the state of get first argument */
            return;
    }
}
/*this function accepts a file name and a suffix and  macro table list (to prevent duplicates in label name)
 *  and builds a parser with the file name and the suffix and the macro table
 *
 * */


parser *build_parser(char *name, char *suffix, macro_unit *macro_table) {
    parser *ptr;
    ptr = allocate_memory(sizeof(parser));

    ptr->scn = build_scanner(name, suffix);
    ptr->label_name = allocate_memory(MAX_LABEL_SIZE + 1);
    ptr->ic = SYSTEM_MEMORY;
    ptr->dc = 0;
    ptr->macro_table = macro_table;

    return ptr;

}
/*this function accepts a parser and frees all the memory that was allocated for the parser
 *
 * */

void free_parser(parser *ptr) {
    free_memory(ptr->label_name);
    free_scanner(ptr->scn);
    free_instruction_list(ptr->head_of_instruction_list);
    free_data_image_list(ptr->head_of_data_list);
    free_label_list(ptr->labels_to_replace_list);
    free_symbol_table(ptr->head_of_symbol_list);
    free_entry_list(ptr->entry_List);
    free_memory(ptr);
}


/*
 * this function accepts a parser and updates the labels that are from the data list by adding the ic to their value
 * */
void update_labels(parser *prs) {
    symbol_unit *current = prs->head_of_symbol_list;
    while (current != NULL) {
        if (current->type == DATA || current->type == STRING) { /*checks if the label is from the data list */
            current->value += prs->ic;   /*if so it adds the ic to the value of the label */

        }
        current = current->next;
    }
}

/*this function accepts a parser and updates the instruction list in every operand defined as index method or direct method
 * according to the value from the final symbol table (and checks if the label is extern and updates the operand) if a symbol is not found it prints an error notification
 * */

BOOLEAN replace_labels(parser *prs) {
    symbol_unit *current, *head = prs->head_of_symbol_list;
    label_to_replace *next = prs->labels_to_replace_list;
    char *name;
    while (next != NULL) {
        name = next->name;
        current = search_in_the_symbol_table(name, head);
        if (current == NULL) {              /*checks if the label is found in the symbol table */
            prs->scn->error_in_file = TRUE; /*if not it updates the variable error in file and  prints an error notification */

            printf("Error in file %s at line %d: No label %s has been defined  \n", prs->scn->file_name, next->line,
                   next->name);


        } else if (current->type == DEFINE) { /*checks if the label is from the type define */

            prs->scn->error_in_file = TRUE;
            printf("Error in file %s at line %d: The label %s assigned is from type define  \n", prs->scn->file_name,  /*if so it updates the variable error in file and  prints an error notification */
                   next->line, next->name);

        } else {                                /*if the label is found in the symbol table updates the instruction list */
            if (next->method == DIRECT_METHOD) {
                next->op->regular_address.address = current->value;
                next->op->regular_address.is_ext = (current->type == EXTERN);
            } else {
                next->op->address_with_index.address = current->value;
                next->op->address_with_index.is_ext = (current->type == EXTERN);
            }
        }

        next = next->next;
    }
    return prs->scn->error_in_file; /*returns if  error in file */
}
/*this function accepts a parser and value and adds the value to the data list
 * */

void add_data_to_the_list(parser *prs, short value) {

    if (prs->head_of_data_list == NULL) {
        prs->current_data = add_data_image(value);
        prs->head_of_data_list = prs->current_data;
        return;
    }
    prs->current_data->next = add_data_image(value);
    prs->current_data = prs->current_data->next;
}
/*this function accepts a parser and a string and checks if the string is a valid label
 * if not it prints an error notification
 * */

BOOLEAN checks_if_string_is_valid_label(parser *prs, char *string) {
    symbol_unit *head, *current;
    macro_unit *mcr;
    BOOLEAN valid ;
    head = prs->head_of_symbol_list;
    current = search_in_the_symbol_table(string, head);
    if (current != NULL) {
        error(prs,"The label %s has already been defined",string); /*checks if the label is already defined in the symbol table */
        return FALSE;
    }
    mcr = search_in_the_macro_table(string, prs->macro_table);
    if (mcr != NULL) {
        error(prs,"The label %s has already been defined as a macro",string);/*checks if the label is already defined in the macro table */
        return FALSE;
    }
    valid =  checks_whether_str_can_be_a_label(string); /*checks if the string can syntactically be a label */
    if(!valid){
        error(prs,"The label %s is invalid",string);

    }
    return valid;
}

