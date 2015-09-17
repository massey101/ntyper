# Name of program
NAME=typer

# Setup Flags
CC=gcc
CFLAGS=-Wall
LFLAGS=-Wall

# Setup files
INCLUDES=-Ilib/
SRC_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(SRC_FILES:.c=.o)))

# Setup Required Directories
DIR=bin obj


# Default option creates directories and then executable
all: $(DIR) bin/$(NAME)

# Link object files
bin/$(NAME): $(OBJ_FILES)
	$(CC) $(LFLAGS) -o $@ $^

# Compile each object file
obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Create the required directories
$(DIR):
	mkdir $@

# Setup for debug mode
debug: CFLAGS+= -g
debug: LFLAGS+= -g
debug: all

# Reset system
clean:
	rm -r bin obj
