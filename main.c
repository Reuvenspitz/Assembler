#include <stdio.h>
#include "assembler.h"

/**
 * This is the main function of the program
 * It gets the file names from the command line and sends them to the assembler
 * @param argc the number of arguments
 * @param argv the arguments
 * @return 0 if the program ran successfully, -1 otherwise
 */


int main(int argc,char *argv[]){
    int  i;
    int error_in_asm = 0;

    if (argc==1){   /*If there are no arguments*/
        printf("Please pass at least one file name\n");
        return -1;
    }
    for (i = 1;i  <argc ; i++) {
        error_in_asm = assembler(argv[i]) || error_in_asm;
    }

    return error_in_asm;
}
