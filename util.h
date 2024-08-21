#ifndef UTIL
#define UTIL
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ctype.h>



#define BASE 10
#define TRUE 1
# define UNKNOWN_VALUE (-12)
#define MAX_LINE_SIZE 82
#define MAX_OP 2
#define  FALSE 0
#define MAX_12_BITS_NUMBER 2047
#define MIN_12_BITS_NUMBER (-2048)
#define MAX_DEFINE_NUMBER 8191
#define MIN_DEFINE_NUMBER (-8192)
#define DEST_BITS 2
#define  SOURCE_BITS 5
#define  SOURCE_BITS_IN_COMMAND 4
#define SYSTEM_MEMORY 100
#define MAX_MEMORY 4096
#define  DATA_BITS 2
#define  OP_BITS 6
#define  OB_WORD 8
#define  OB_BASE 4

#define MAX_LABEL_SIZE 32
enum Directive{ERROR_STATE =-1, DATA,STRING,ENTRY,EXTERN,DEFINE,CODE};
typedef enum Directive directive;

typedef int BOOLEAN;



void * allocate_memory(int size);
void * reallocate_memory(void *ptr,size_t size);
void free_memory(void* ptr);

FILE * open_file_to_read(char *name,char *suffix);
FILE * open_file_to_write(char *name,char *suffix);

BOOLEAN is_empty_line(const char *line);
BOOLEAN is_a_comment(const char *line);
BOOLEAN is_a_label(char *str);


directive find_statement_type(char *str);

BOOLEAN converts_a_string_to_a_number(char *str,long *num);

BOOLEAN is_a_index_method(char *str);


#endif

