#ifndef MACRO_HANDLING_H
#define MACRO_HANDLING_H
#include "util.h"
#include "macro.h"
#include "scanner.h"
#include "instruction.h"

macro_unit * preprocessor(char* name,BOOLEAN *error_in_preprocessor);

#endif
