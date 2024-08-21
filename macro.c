#include "macro.h"


macro_unit* build_new_macro(char *name,macro_unit *head){
    macro_unit *pointer = allocate_memory(sizeof ( macro_unit));
    pointer->name= allocate_memory(sizeof (name)+1);
    pointer->body = allocate_memory(MAX_LINE_SIZE);
    memset(pointer->body,0,MAX_LINE_SIZE);
    strcat(pointer->name,name);
    pointer->size_of_body = 0;
    pointer->next = head;

    return pointer;
}
void free_macro_table(macro_unit *mcr){
    macro_unit *current,*next;
    current = mcr;
    while (current!=NULL){
        next = current->next;
        free_memory(current->name);
        free_memory(current->body);
        free_memory(current);
        current =next;
    }
}


macro_unit* search_in_the_macro_table(char* name,macro_unit* head){
    macro_unit *current_macro = head;

    while (current_macro != NULL){
        if (strcmp(name,current_macro->name)==0)
            return current_macro;
        else
            current_macro = current_macro->next;
    }

    return NULL;
}
void add_line_to_macro(macro_unit *mcr,char *line){
    if (mcr==NULL){
        printf("error pas null");
        exit(EXIT_FAILURE);
    }
    mcr->size_of_body = mcr->size_of_body+MAX_LINE_SIZE;
    mcr->body = reallocate_memory(mcr->body,mcr->size_of_body);
    strcat(mcr->body,line);
    strcat(mcr->body,"\n");
}
BOOLEAN checks_whether_str_can_be_a_macro(char *str,macro_unit *head){
    macro_unit *current =NULL;
    current = search_in_the_macro_table(str,head);
    if (current!=NULL){
        return FALSE;
    }

    return isalpha(str[0]);
}
