# Name of the project
PROJ_NAME=./bin/bbRick
 
# .c files
CPP_SOURCE=$(wildcard ./*.cpp)
 
# .h files
H_SOURCE=$(wildcard ./*.h)
 
# Object files
OBJ=$(subst .cpp,.o,$(subst ./,obj/,$(CPP_SOURCE)))
 
# Compiler and linker
CC=g++
 
# Flags for compiler
CC_FLAGS=-Wall \
         -pthread -lrt
 
# Command used at clean target
RM = rm -rf
 
#
# Compilation and linking
#
all: objFolder binFolder $(PROJ_NAME)
 
$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $(CC_FLAGS) $^ -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '
 
./obj/%.o: ./%.cpp 
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $(CC_FLAGS) -c $< -o $@
	@ echo ' '
 
./obj/main.o: ./main.cpp $(H_SOURCE)
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

.PHONY: clean all

