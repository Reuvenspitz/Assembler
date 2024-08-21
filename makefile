Compiler=gcc
CompilerFlags=-ansi -Wall -pedantic

ProjectName = reuven
ObjectFiles = 	main.o \
				assembler.o \
				instruction.o \
				macro.o \
				macro_handling.o \
				parser.o \
				scanner.o \
				util.o

$(ProjectName): $(ObjectFiles)
	$(Compiler) $(CompilerFlags) $(ObjectFiles) -o $(ProjectName)

%.o: %.c
	$(Compiler) $(CompilerFlags) -c $<

test:
	./$(ProjectName) tests/demo /errors

clean:
	rm -rf $(ProjectName)
	rm -rf *.o