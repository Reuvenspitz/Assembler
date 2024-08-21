#include "util.h"

/* this function allocates memory according to the variable size
 * and checks that the memory allocation was successful
 * if not the function prints error report and stops the program*/
void *allocate_memory(int size) {
    char *pointer;
    pointer = calloc(size, 1);
    if (pointer == NULL) {   /*if pointer is null memory allocation failed */
        perror("Memory allocation failed  \n ");
        exit(EXIT_FAILURE);
    }
    return pointer;

}

/*this function receives a pointer to a block of memory and a variable named size
 * and reallocates memory according to size of "size" and the pointer ptr
 * and checks that the memory allocation was successful
 * if not the function prints error report and stops the program */
void *reallocate_memory(void *ptr, size_t size) {
    char *new = NULL;
    new = realloc(ptr, size);
    if (new == NULL) {
        printf("memory error");
        exit(EXIT_FAILURE);
    }
    return new;
}

/*
 * This function gets a pointer to memory and frees it
 */
void free_memory(void *ptr) {
    if (ptr == NULL) {
        printf("can't free null ptr");
        exit(EXIT_FAILURE);
    }
    free(ptr);
}

/*This function accepts a name and an extension and opens
 * a file  for reading with the name and extension
 * and check that the file opens well If so returns a pointer to the file
 * and if not the function prints error report and stops the program
 * */

FILE *open_file_to_read(char *name, char *suffix) {
    FILE *pointer;
    char *file_name = (char *) allocate_memory((int) (strlen(name) + strlen(suffix) + 1));
    strcpy(file_name, name);
    strcat(file_name, suffix); /*Copy the name and the suffix */
    pointer = fopen(file_name, "r");
    if (pointer == NULL) {    /*Check that the file is opened properly */
        exit(EXIT_FAILURE);
    }
    free_memory(file_name); /* Frees the memory allocated to the file name*/
    return pointer;
}

/*This function accepts a name and an extension and opens
 * a file  for writing with the name and extension
 * and check that the file opens well If so returns a pointer to the file
 * and if not the function prints error report and stops the program
 * */
FILE *open_file_to_write(char *name, char *suffix) {
    FILE *pointer;
    char *file_name = (char *) allocate_memory((int) (strlen(name) + strlen(suffix) + 1));
    strcpy(file_name, name);
    strcat(file_name, suffix);        /*Copy the name and the suffix */
    pointer = fopen(file_name, "w");
    if (pointer == NULL) {
        perror("Couldn't open new file \n");
        exit(EXIT_FAILURE);
    }
    free_memory(file_name); /* Frees the memory allocated to the file name*/
    return pointer;
}

/*
 * This function checks whether there is a label definition at the beginning of a line
 * by checking whether the first word ends with a colon
 * */
BOOLEAN is_a_label(char *str) {

    return (*(str + strlen(str) - 1) == ':');
}

/*
 *This function receives a pointer to a string,
 * checks whether the string represents a number and converts it to a number
 *
 * */
BOOLEAN converts_a_string_to_a_number(char *str, long *num) {
    char *per;
    *num = strtol(str, &per, BASE);
    if (*per != '\0') /* Checks whether there are characters that are not part of the number in the string*/
        return FALSE;

    return TRUE;
}

BOOLEAN checks_string_is_a_is_valid_label(char *str) {
    size_t i = 0;
    if (strlen(str) > MAX_LABEL_SIZE) {
        return FALSE;
    }

    if (!isalpha(*str))
        return FALSE;
    for (; i < strlen(str); i++) {
        if (!isalpha(str[i]) && !isdigit(str[i]))
            return FALSE;

    }

    return TRUE;

}

/*
 *
 * */
BOOLEAN is_a_index_method(char *str) {
    size_t length = strlen(str);
    return length && str[length - 1] == '[';
}

/*
 *This function accepts a pointer to a string of characters and checks
 * whether the string is empty by checking whether the first character is zero
 * */
BOOLEAN is_empty_line(const char *str) {
    return *str == 0;

}

/*
 * This function checks whether a line is a comment by checking whether the
 * first character is a semicolon*/
BOOLEAN is_a_comment(const char *line) {

    return *line == ';';
}




