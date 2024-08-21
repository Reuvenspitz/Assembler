#include "assembler.h"

/*This function is the first pass of the assembler
 * It reads the file line by line and parse it
 * It ignores comments and empty lines
 * It separates the line into tokens and parse them
 * It returns the parser
 * */
parser * first_pass(char *name,macro_unit *macro_table) {
    parser *prs;

    prs = build_parser(name, ".am",macro_table);


    while (!prs->scn->end_of_file) {
        next_line_in_parser(prs);

        if (is_a_comment(prs->scn->current_line)|| is_empty_line(prs->scn->current_line)) { /*ignore comments and empty lines */
            continue;
        }
        while (!prs->scn->end_of_line && !prs->ignore_line) { /*parse the line */
            separate_token_from_line(prs->scn);
            parse_token(prs); /*parse the token */
        }
    }
    return prs;
}


/**
 * this function prints the word in the ob file according to the format that was given in the project instructions
 *
 *
 */
void print_word(unsigned short data,int ic,FILE *file){
    size_t i = 0;
    char temp;
    char ob_word[OB_WORD];
    char letters[] = {'*', '#', '%', '!'}; /*letters to represent the numbers in the ob file*/

    while (data != 0)
    {
        ob_word[i++] = letters[data % OB_BASE];  /*convert the number to the base 4*/
        data /= OB_BASE;
    }
    while (i < 7)
    {
        ob_word[i++] = letters[0];    /*fill the rest of the word with '*' */
    }
    for (i = 0; i < 3; i++)      /*reverse the word*/
    {
        temp = ob_word[i];
        ob_word[i] = ob_word[6 - i];
        ob_word[6-i] = temp;
    }
    ob_word[7]=0;   /*add the null terminator*/
    fprintf(file,"%04d %s\n", ic, ob_word); /*print the word to the file*/
}
/**
 * this function prints instruction  to the ob file according to the format that was given in the project instructions
 *
 */

void  print_instruction(instruction *ins,FILE *file){
    unsigned short data;
    int current_ic = ins->ic;
    data=0;

    data |= ((ins->dest_mode) << DEST_BITS);         /*add the destination mode to the word*/
    data |= ((ins->source_mode) << SOURCE_BITS_IN_COMMAND);/*add the source mode to the word*/
    data |= ((ins->opcode) << OP_BITS);                 /*add the opcode to the word*/

    print_word(data, current_ic, file);      /*print the word to the file*/
    data =0;
    current_ic++;             /*increment the ic*/

    if(!ins->num_of_operands){  /*if there are no operands return*/
        return;
    }
    if (ins->num_of_operands == MAX_OP && ins->source_mode == REG_METHOD && ins->dest_mode == REG_METHOD){  /*if there are two registers as operands*/
        data |= (ins->dest.reg << DEST_BITS);
        data |= (ins->source.reg << SOURCE_BITS);
        print_word(data, current_ic, file);    /*print the word in the special format to that case to the file and return*/
        return;

    }
    if (ins->num_of_operands == MAX_OP){         /* prints the operands  according to the project instruction*/

        if (ins->source_mode == IMMEDIATE_METHOD){

            data |=(ins->source.immediate<<DATA_BITS);
            print_word(data,current_ic,file);

        }else if (ins->source_mode == REG_METHOD){

            data |=(ins->source.reg<<SOURCE_BITS);
            print_word(data,current_ic,file);

        } else if (ins->source_mode == DIRECT_METHOD){
            data =1;
            if(!ins->source.regular_address.is_ext){
                data++;
            }
            data |= (ins->source.regular_address.address<<DATA_BITS);
            print_word(data,current_ic,file);

        } else if (ins->source_mode == INDEX_METHOD){

            data =1;
            if (!ins->source.address_with_index.is_ext){
                data++;
            }
            data |= (ins->source.address_with_index.address<<DATA_BITS);
            print_word(data,current_ic,file);

            data = 0;
            current_ic++;
            data |= (ins->source.address_with_index.offset<<DATA_BITS);
            print_word(data,current_ic,file);

        }
        data = 0 ;
        current_ic++;

    }
    if (ins->dest_mode == IMMEDIATE_METHOD){

        data |= (ins->dest.immediate<<DEST_BITS);
        print_word(data,current_ic,file);

    }else if (ins->dest_mode == REG_METHOD){

        data |=(ins->dest.reg<<DEST_BITS);
        print_word(data,current_ic,file);

    } else if (ins->dest_mode == DIRECT_METHOD){
        data =1;
        if(!ins->dest.regular_address.is_ext){
            data++;
        }
        data |= (ins->dest.regular_address.address<<DEST_BITS);
        print_word(data,current_ic,file);

    } else if (ins->dest_mode == INDEX_METHOD){

        data =1;
        if (!ins->dest.address_with_index.is_ext){
            data++;
        }
        data |= (ins->dest.address_with_index.address<<DEST_BITS);
        print_word(data,current_ic,file);

        data = 0;
        current_ic++;
        data |= (ins->dest.address_with_index.offset<<DEST_BITS);
        print_word(data,current_ic,file);

    }
}
/**
 * this function creates and prints the ob file according to the format that was given in the project instructions
 *
 */

void print_ob_file(parser *prs){
    FILE *ob_file;
    instruction  *current_inst ;
    data_image  *current_data;
    int i;

    ob_file = open_file_to_write(prs->scn->file_name,".ob"); /*open the ob file*/
    current_inst = prs->head_of_instruction_list;
    current_data = prs->head_of_data_list;


    fprintf(ob_file,"%5d %d\n",prs->ic-SYSTEM_MEMORY,prs->dc); /*print the ic and dc*/
        while (current_inst != NULL){                  /*print the instructions list*/
        print_instruction(current_inst, ob_file);
        current_inst = current_inst->next;
    }
    for (i=0;i<prs->dc;i++){                      /*print the data list*/
        print_word(current_data->data,prs->ic+i,ob_file);
        current_data =current_data->next;
    }

    fclose(ob_file);   /*close the file*/
}
/**
 * this function creates and prints the ext file according to the format that was given in the project instructions
 *
 */
void print_ext_file(parser *prs){
    FILE *ext_file;
    label_to_replace *current;
    symbol_unit *current_label,*head=prs->head_of_symbol_list;
    current = prs->labels_to_replace_list;

    if (current ==NULL){   /*if there are no extern labels return*/
        return;

    }
    ext_file = open_file_to_write(prs->scn->file_name,".ext"); /*open the ext file*/
    while (current!=NULL){
        current_label = search_in_the_symbol_table(current->name,head);
        if (current_label && current_label->type==EXTERN){                  /*print the extern labels*/
            fprintf(ext_file,"%s %04d\n",current->name,current->ic);
        }
        current =current->next;

    }
    fclose(ext_file); /*close the file*/


}
/**
 * this function checks if the entry labels are exist in the symbol table and prints them to the ent
 * file according to the format that was given in the project instructions
 * if there is an error in the file it deletes the file and returns TRUE e
 * otherwise it returns FALSE
 */
BOOLEAN check_and_print_ent_file(parser *prs){
    FILE *ent_file;
    BOOLEAN error_in_file=FALSE;
    char name[MAX_LINE_SIZE];
    symbol_unit *current_label,*head=prs->head_of_symbol_list;
    entry_list *current = prs->entry_List;

    memset(name, 0, MAX_LINE_SIZE);

    if(current==NULL){   /*if there are no entry labels return FALSE */
        return FALSE;
    }
    ent_file = open_file_to_write(prs->scn->file_name, ".ent"); /*open the ent file*/
    while (current != NULL){
        current_label = search_in_the_symbol_table(current->name,head);
        if (current_label == NULL){
            printf("Error in file %s at line %d: No label %s has been defined  \n", prs->scn->file_name,current->line,current->name);
            error_in_file =TRUE;
            current = current->next;
            continue;
        }
        if (current_label->type==EXTERN){  /*if the label is extern print an error message and return TRUE*/
            printf("Error in file %s at line %d:  label %s has been defined as extern and entry in the  \n", prs->scn->file_name,current->line,current->name);
            error_in_file =TRUE;
            current = current->next;
            continue;
        }
        if (!prs->scn->error_in_file){   /*print the entry labels*/
            fprintf(ent_file,"%s %04d\n",current->name,current_label->value);}
        current = current->next;
    }
    if(error_in_file || prs->scn->error_in_file){ /*if there is an error in the file delete the file and return TRUE*/
        strcat(name,prs->scn->file_name);
        strcat(name,".ent");
        remove(name);
        return error_in_file;
    }
    fclose(ent_file); /*close the file*/
    return error_in_file;

}
/**
 * this function running the assembler on the file
 * it calls the preprocessor function and the first pass function
 *  it updates the labels and replace the labels in the instructions list
 *  it checks the entry labels
 *  if there is an error in the file it prints an error message
 *  otherwise it prints the ob and ext and ent files
 */

BOOLEAN assembler(char *name) {

    BOOLEAN error_in_assembling ;
    macro_unit *macro_table = NULL;
    parser *prs = NULL;
    error_in_assembling = FALSE;

    macro_table = preprocessor(name,&error_in_assembling);
    prs = first_pass(name,macro_table); /*first pass*/
    update_labels(prs);
    prs->scn->error_in_file = replace_labels(prs) || prs->scn->error_in_file; /*replace the labels in the instructions list*/
    prs->scn->error_in_file = check_and_print_ent_file(prs) || prs->scn->error_in_file; /*check the entry labels and print them to the ent file*/

     if (prs->scn->error_in_file   ) {
        printf("couldn't create ob ext and ent file for file %s\n", prs->scn->file_name);/*if there is an error in the file print an error message*/
        error_in_assembling = prs->scn->error_in_file;
    } else if (prs->ic+prs->dc>MAX_MEMORY){ /*if the memory is too big print an error message and return*/
         printf("Error in file %s: The memory is too big\n", prs->scn->file_name);
         prs->scn->error_in_file = TRUE;
     }
     else {                            /*print the ob and ext files*/
        print_ob_file(prs);
        print_ext_file(prs);
        printf("Assembly process on file %s.as passed successfully\n", prs->scn->file_name);
    }
    free_macro_table(macro_table);/*free the macro table*/
    free_parser(prs);   /*free the parser*/

    return error_in_assembling;
}
