#include "instruction.h"
/*This function is for adding an instruction to the list of instructions
 * It gets the opcode, the ic of the instruction and the number of operands
 * It returns a pointer to the instruction
 * */
instruction *add_instruction(opcodes op, int ic, int num_of_operands) {
    instruction *a;
    a = allocate_memory(sizeof(instruction));
    a->opcode = op;
    a->next = NULL;
    a->ic = ic;
    a->num_of_operands = num_of_operands;
    a->source_mode = IMMEDIATE_METHOD;
    a->dest_mode = IMMEDIATE_METHOD;

    return a;
}
/*This function is for freeing the memory of the list of instructions
 * It gets the head of the list and frees the memory of each instruction
 * */
void free_instruction_list(instruction *head) {
    instruction *current, *next;
    current = head;

    while (current != NULL) {
        next = current->next;
        free_memory(current);
        current = next;
    }
}
/*This function is for adding a data image to the list of data images
 * It gets the value of the data and returns a pointer to the data image
 * */
data_image *add_data_image(short value) {
    data_image *d;
    d = allocate_memory(sizeof(data_image));
    d->data = value;
    d->next = NULL;
    return d;
}
/*This function is for freeing the memory of the list of data images
 * It gets the head of the list and frees the memory of each data image
 * */
void free_data_image_list(data_image *head) {
    data_image *current, *next;
    current = head;

    while (current != NULL) {
        next = current->next;
        free_memory(current);
        current = next;
    }
}
/*This function is for adding a label to the list of labels that should be replaced
 * It gets the name of the label, the operand that should be replaced, the head of the list, the ic of the operand, the line number and the addressing method
 * It returns a pointer to the label to replace
 * */

label_to_replace *add_label_to_replace(char *name, operand *op,
                                       label_to_replace *head, int ic, int line,
                                       Addressing_method method) {
    label_to_replace *lab;
    lab = allocate_memory(sizeof(label_to_replace));
    lab->op = op;
    memset(lab->name, 0, MAX_LABEL_SIZE);
    strcat(lab->name, name);
    lab->ic = ic;
    lab->line = line;
    lab->next = head;
    lab->method = method;
    return lab;
}
/*This function is for freeing the memory of the list of labels that should be replaced
 * It gets the head of the list and frees the memory of each label to replace
 * */
void free_label_list(label_to_replace *head) {

    label_to_replace *current, *next;
    current = head;

    while (current != NULL) {
        next = current->next;
        free_memory(current);
        current = next;
    }
} /*This function is for finding  if string is a register and if so the register number
 * It gets a string returns the register number or error register if it is not a register
 * */
Reg find_register(char *token) {
    static struct {
        const char *register_name;
        enum reg register_number;
    } register_scan_table[] = {
            {"r0", R0}, {"r1", R1}, {"r2", R2}, {"r3", R3},
            {"r4", R4}, {"r5", R5}, {"r6", R6}, {"r7", R7},
    };

    int i;

    for (i = 0; i < ERROR_REG; i++) {      /*ERROR_REG is the number of registers*/
        if (strcmp(register_scan_table[i].register_name, token) == 0) { /*If the string is a register*/
            return register_scan_table[i].register_number;           /*Return the register number*/
        }
    }

    return ERROR_REG;   /*If the string is not a register return error register*/
}
/*This function is for finding the type of the statement
 * It gets a string and returns the type of the statement
 * */
directive find_statement_type(char *str) {
    if (strcmp(str, ".data") == 0)
        return DATA;
    else if (strcmp(str, ".string") == 0)
        return STRING;
    else if (strcmp(str, ".entry") == 0)
        return ENTRY;
    else if (strcmp(str, ".extern") == 0)
        return EXTERN;
    else if (strcmp(str, ".define") == 0)
        return DEFINE;
    else if (find_command(str).opcode == ERROR_OPCODE) /*If the string is not a command*/
        return ERROR_STATE;
    else
        return CODE;
}
/*This function is for finding the command
 * It gets a string and returns the command
 * */
command_with_info find_command(char *token) {
    static struct {
        const char *opcode_name;
        command_with_info result;
    } command_with_info_table[] = {
            {"mov",
                    {MOV, MAX_OP, {TRUE, TRUE, TRUE, TRUE}, {FALSE, TRUE, TRUE, TRUE}}},
            {"cmp",
                    {CMP, MAX_OP, {TRUE, TRUE, TRUE, TRUE}, {TRUE, TRUE, TRUE, TRUE}}},
            {"add",
                    {ADD, MAX_OP, {TRUE, TRUE, TRUE, TRUE}, {FALSE, TRUE, TRUE, TRUE}}},
            {"sub",
                    {SUB, MAX_OP, {TRUE, TRUE, TRUE, TRUE}, {FALSE, TRUE, TRUE, TRUE}}},
            {"not",
                    {NOT, 1, {FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
            {"clr",
                    {CLR, 1, {FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
            {"lea",
                    {LEA, MAX_OP, {FALSE, TRUE, TRUE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
            {"inc",
                    {INC, 1, {FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
            {"dec",
                    {DEC, 1, {FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
            {"jmp",
                    {JMP, 1, {FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, FALSE, TRUE}}},
            {"bne",
                    {BNE, 1, {FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, FALSE, TRUE}}},
            {"red",
                    {RED, 1, {FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
            {"prn", {PRN, 1, {FALSE, FALSE, FALSE, FALSE}, {TRUE, TRUE, TRUE, TRUE}}},
            {"jsr",
                    {JSR, 1, {FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, FALSE, TRUE}}},
            {"rts",
                    {RTS, 0, {FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}}},
            {"hlt",
                    {HLT, 0, {FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}}},
            {"error",
                    {ERROR_OPCODE,
                          0,
                                  {FALSE, FALSE, FALSE, FALSE},
                            {FALSE, FALSE, FALSE, FALSE}}},
    };

    int i;

    for (i = 0; i < ERROR_OPCODE; i++) {
        if (strcmp(command_with_info_table[i].opcode_name, token) == 0) {
            return command_with_info_table[i].result;
        }
    }

    return command_with_info_table[ERROR_OPCODE].result;
}
/*This function is for adding a symbol to the symbol table
 * It gets the name of the symbol and the current symbol table
 * It returns a pointer to the new symbol table
 * */

symbol_unit *add_symbol(char *name, symbol_unit *current) {

    symbol_unit *new = allocate_memory(sizeof(symbol_unit));

    memset(new->name, 0, MAX_LABEL_SIZE + 1);
    strcat(new->name, name);
    new->value = 0;
    new->next = current;
    return new;
}
/*This function is for freeing the memory of the symbol table
 * It gets the head of the symbol table and frees the memory of each symbol
 * */
void free_symbol_table(symbol_unit *head) {
    symbol_unit *current, *next;
    current = head;
    while (current != NULL) {
        next = current->next;
        free_memory(current);
        current = next;
    }
}
/*This function is for searching in the symbol table
 * It gets the name of the symbol and the head of the symbol table
 * It returns a pointer to the symbol if it is found and NULL if it is not found
 *
 * */
symbol_unit *search_in_the_symbol_table(char *name, symbol_unit *head) {
    symbol_unit *current = head;
    while (current != NULL) {
        if (strcmp(name, current->name) == 0)
            return current;
        else
            current = current->next;
    }
    return NULL;
}
/*This function is for setting the type of the symbol
 * It gets a pointer to the symbol and the type of the symbol
 * */
void set_symbol_type(symbol_unit *symbol, directive type) {
    symbol->type = type;
}
/*This function is for setting the value of the symbol
 * It gets a pointer to the symbol and the value of the symbol
 * */
void set_symbol_value(symbol_unit *symbol, int value) { symbol->value = value; }
/*This function is for adding an entry to the list of entries
 * It gets the head of the list, the name of the entry and the line number
 * It returns a pointer to the new list
 * */
entry_list *add_entry(entry_list *head, char *name, int line) {
    entry_list *current;
    current = allocate_memory(sizeof(entry_list));
    memset(current->name, 0, MAX_LABEL_SIZE);
    strcpy(current->name, name);
    current->line = line;
    current->next = head;

    return current;
}
/*This function is for checking whether a string can be a label
 * It gets a string and returns TRUE if it can be a label and FALSE if it can not be a label
 * */
BOOLEAN checks_whether_str_can_be_a_label(char *token) {
    Reg r;
    command_with_info c;
    size_t i = 0;
    r = find_register(token);
    c = find_command(token);

    if (r != ERROR_REG || c.opcode != ERROR_OPCODE || strcmp(token, "mcr") == 0 || /*If the string is reserved word */
        strlen(token) > MAX_LABEL_SIZE) {                    /*Or the length of the string is more than the maximum label size*/
        return FALSE;
    }

    if (!isalpha(*token))  /*If the first character is not a letter*/
        return FALSE;

    for (; i < strlen(token); i++) {              /*If the string contains characters that are not letters or digits*/
        if (!isalpha(token[i]) && !isdigit(token[i]))
            return FALSE;
    }

    return TRUE;
}
/*This function is for freeing the memory of the list of entries
 * It gets the head of the list and frees the memory of each entry
 * */
void free_entry_list(entry_list *head) {
    entry_list *temp = head;

    while (head) {
        temp = head->next;
        free_memory(head);
        head = temp;
    }
}
