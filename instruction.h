#ifndef MMN_14_INSTRUCTION_H
#define MMN_14_INSTRUCTION_H

#include "util.h"

#define MAX_MODES_FOR_OPERAND 4




enum opcodes
{
    MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, HLT, ERROR_OPCODE
};
typedef enum opcodes opcodes;

enum reg

{
    R0, R1, R2, R3, R4, R5, R6, R7, ERROR_REG
};
typedef enum reg Reg;
enum addressing_method{IMMEDIATE_METHOD, DIRECT_METHOD ,INDEX_METHOD ,REG_METHOD ,NON};
typedef enum addressing_method Addressing_method;
/*This linked list is for the symbol table with the following fields
 * name value and type(data define and so on)
 * */
struct Symbol_unit{
    char name[MAX_LABEL_SIZE+1] ;
    int value;
    directive type;
    struct Symbol_unit *next;
};
typedef struct Symbol_unit symbol_unit;


/*This struct is for the operands of the instruction
 * It can be a register, an immediate value, a regular address or an address with an index
 * */
typedef union {
        struct {
           int address;
           BOOLEAN is_ext;
         }regular_address;
        int  immediate;
        enum reg reg;
        struct {
            int address;
            int offset;
            BOOLEAN is_ext;
        }address_with_index;
} operand;
/*This struct is for the instruction
 * It has the opcode, the number of operands, the addressing methods for the source and the destination
 * the source and the destination
 * the ic of the instruction
 * and operands for the source and the destination
 * */
typedef struct instruction
{
    Addressing_method source_mode;
    Addressing_method dest_mode;
    opcodes opcode;
    operand source;
    operand dest;
    int num_of_operands;
    int ic ;
    struct instruction *next;
} instruction;


/*This struct makes it possible to provide information about instruction
 * the op code number of operands and the permitted addressing methods
 *
 * */
typedef struct {
    enum opcodes opcode;
    int num_of_operands;
    BOOLEAN source_modes[MAX_MODES_FOR_OPERAND];
    BOOLEAN dest_modes[MAX_MODES_FOR_OPERAND];
} command_with_info;


/*This is a linked list to save the labels that should be replaced with the following fields
 * name : The label name
 * ic: The ic xf the operand for the extern file(If this label turns out to be external)
 * line : for error messages
 *  The address method and the operand for the replacement
 * */
struct label_to_replace
{
    char name[MAX_LABEL_SIZE];
    int ic;
    int line;
    Addressing_method method;
    operand* op;
    struct label_to_replace* next;
};
/*This is a linked list to save the memory image*/
struct data_image{
    short data;
    struct data_image *next;
};
typedef struct data_image  data_image;

typedef struct label_to_replace label_to_replace;
/*This linked list is for saving the list of labels defined as entries and
 * their line number for future error message
 * */
 struct entry_list{
     char  name[MAX_LABEL_SIZE];
     int line;
     struct entry_list *next;
};
typedef struct entry_list  entry_list;

entry_list* add_entry(entry_list *head,char *name,int line);
void free_entry_list(entry_list *head);



data_image* add_data_image(short value);
void free_data_image_list(data_image *head);

label_to_replace* add_label_to_replace(char* name, operand * op, label_to_replace *head,int ic,int line,Addressing_method method);
void free_label_list(label_to_replace *head);


instruction *add_instruction( opcodes op,int ic,int num_of_operands);
void free_instruction_list(instruction *head);

enum reg find_register(char *token);
command_with_info find_command(char *token);


symbol_unit * add_symbol(char *name, symbol_unit *current);
void free_symbol_table(symbol_unit *head);
symbol_unit * search_in_the_symbol_table(char *name,symbol_unit *head);
void set_symbol_type(symbol_unit *symbol,directive type);
void set_symbol_value(symbol_unit *symbol,int value);

BOOLEAN checks_whether_str_can_be_a_label(char *token);






#endif
