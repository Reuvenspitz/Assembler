# Assembler in C

A two-pass assembler written in C, developed as the final project for the Open University course **20465 – System Programming Laboratory**.

The project implements a complete assembly pipeline for a custom assembly language, including macro preprocessing, lexical scanning, parsing, symbol-table construction, instruction and data encoding, label resolution, and output-file generation.

## Overview

The assembler processes each source file in several stages:

1. **Preprocessing**
   - Reads the original source file (`.as`)
   - Expands user-defined macros
   - Produces an intermediate file (`.am`)

2. **First Pass**
   - Parses the preprocessed source
   - Builds the symbol table
   - Encodes instructions and data
   - Tracks unresolved label references

3. **Resolution and Validation**
   - Updates data-label addresses
   - Replaces symbolic references with final addresses
   - Validates `.entry` and `.extern` declarations

4. **Output Generation**
   - Produces final assembler output files

## Supported Assembly Features

### Directives

The assembler supports the following directives:

- `.data`
- `.string`
- `.entry`
- `.extern`
- `.define`

### Instruction Set

The implemented instruction set includes:

- `mov`
- `cmp`
- `add`
- `sub`
- `not`
- `clr`
- `lea`
- `inc`
- `dec`
- `jmp`
- `bne`
- `red`
- `prn`
- `jsr`
- `rts`
- `hlt`

### Addressing Modes

The code supports four addressing modes:

- **Immediate**
- **Direct**
- **Index**
- **Register**

## Output Files

For each successfully assembled source file, the program may generate:

- `filename.ob` — object file
- `filename.ext` — external symbol references
- `filename.ent` — entry symbols

The assembler also creates an intermediate preprocessed file:

- `filename.am` — macro-expanded source

## Object Encoding

The assembler encodes machine words and writes them to the object file in a custom **base-4 representation**, using the symbols:

- `*`
- `#`
- `%`
- `!`

## Project Structure

### Core Flow

- `main.c`  
  Program entry point. Receives input file names from the command line and passes them to the assembler pipeline.

- `assembler.c / assembler.h`  
  Implements the high-level flow of the assembler: preprocessing, first pass, label resolution, entry validation, and output generation.

### Parsing and Lexical Processing

- `scanner.c / scanner.h`  
  Tokenization and line-scanning utilities.

- `parser.c / parser.h`  
  Parsing logic for labels, directives, operands, and instructions.

- `instruction.c / instruction.h`  
  Defines the instruction set, addressing modes, registers, and instruction metadata.

### Macro Preprocessing

- `macro.c / macro.h`  
  Macro table data structures and macro management logic.

- `macro_handling.c / macro_handling.h`  
  Handles macro expansion and creation of the intermediate `.am` file.

### Utilities

- `util.c / util.h`  
  Shared helper functions, constants, file handling, validation, and general utilities.

### Build Configuration

- `makefile`  
  Builds the project using `gcc` with ANSI C and strict warning flags.

## Build

Compile the project with:

~~~bash
make
~~~

This creates the executable:

~~~bash
./reuven
~~~

## Usage

Run the assembler with one or more file names **without the `.as` suffix**:

~~~bash
./reuven program1 program2
~~~

For example, if the source file is:

~~~text
program1.as
~~~

you should run:

~~~bash
./reuven program1
~~~

If no file names are provided, the program prints an error message.

## Design Notes

This project was built in a modular way, separating the assembler into distinct stages and components:

- preprocessing
- scanning
- parsing
- instruction handling
- macro management
- output generation

## Design Notes

This project was built in a modular way, separating the assembler into distinct stages and components:

- preprocessing
- scanning
- parsing
- instruction handling
- macro management
- output generation

The implementation emphasizes:

- clear separation of responsibilities between modules
- structured multi-stage assembly
- explicit error reporting
- maintainable C code in a Linux environment
- state-based parsing logic in the main analysis flow, inspired by automata-oriented thinking

## What I Learned

Through this project I gained hands-on experience with:

- multi-stage program design
- parsing and low-level text processing
- symbol resolution
- compiler/assembler-style workflows
- modular software design in C
- building and debugging larger system-level programs
- applying state-machine style reasoning to parsing and control flow
