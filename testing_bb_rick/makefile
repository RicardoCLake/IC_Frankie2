# Name of the project
PROJ_NAME=./bin/testingbbRick
 
# Compiler and linker
CC=g++
 
# Flags for compiler
CC_FLAGS=-Wall \
 
# Command used at clean target
RM = rm -rf
 
#
# Compilation and linking
#
all: objFolder binFolder $(PROJ_NAME)
 
$(PROJ_NAME): ./obj/main.o ./obj/Gpio.o
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $(CC_FLAGS) $^ -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '
 
./obj/Gpio.o: ../bb_rick/Gpio.cpp
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $(CC_FLAGS) -c $< -o $@
	@ echo ' '
 
./obj/main.o: ./main.cpp ./header.h ../bb_rick/Gpio.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $(CC_FLAGS) -c $< $(CC_FLAGS) -o $@
	@ echo ' '
 
objFolder:
	@ mkdir -p obj

binFolder:
	@ mkdir -p bin
 
clean:
	@ $(RM) ./obj/*.o $(PROJ_NAME) *~
	@ rmdir obj

soft-clean:
	@ $(RM) ./obj/*.o *~
	@ rmdir obj

.PHONY: all clean

