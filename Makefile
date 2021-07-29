CC=gcc
CFLAGS=-c -g -Wall

AR=ar
ARFLAGS=-rc

# Source directory
SRC=Src
TEST_SRC=tests

OBJ=build/obj
TEST_OBJ=tests/build/obj

# Returns all files with *.c extension from $(SRC) directory.
SRCS=$(wildcard $(SRC)/*.c)
TEST_SRCS=$(wildcard $(TEST_SRC)/*.c)

# This takes $(SRCS) as input,
# if the $(SRC)/%.c pattern is matched,
# then it is changed to $(OBJ)/%.o and added to return value.
# This way, it converts the list of C files specified in SRCS to list of .o
# files, to build object files.
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
TEST_OBJS=$(patsubst $(TEST_SRC)/%.c, $(TEST_OBJ)/%.o, $(TEST_SRCS))

BINDIR=build/bin
BINNAME=ring
BIN=$(BINDIR)/lib$(BINNAME)

TEST_BINDIR=tests/build/bin
TEST_BINNAME=test
TEST_BIN=$(TEST_BINDIR)/$(TEST_BINNAME)

LIBDIR=lib

TEST_LD_FLAGS=-I$(LIBDIR)  -L$(LIBDIR) -l$(BINNAME) -lcriterion
STATICLIB=$(BIN).a

CRITERION_FLAGS=--verbose --full-stats

init:
	mkdir -p $(TEST_BINDIR)
	mkdir -p $(TEST_OBJ)
	mkdir -p $(BINDIR)
	mkdir -p $(OBJ)

all: lib test

lib: $(BIN)
	cp $(BINDIR)/* $(LIBDIR)/
	cp $(SRC)/*.h $(LIBDIR)/

# Compiles static library
$(BIN): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) -o $@
	$(AR) $(ARFLAGS) $(BIN).a $(OBJS)

# Creates object files from c files
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: lib $(TEST_BIN)
	$(info Running unit tests...)
	$(TEST_BIN) $(CRITERION_FLAGS)
# $(CC) $(TEST_CFLAGS) -c tests/test.c -o tests/test.o

$(TEST_BIN): $(TEST_OBJS)
	$(CC) $(TEST_CFLAGS) $(TEST_OBJS) -o $@ $(TEST_LD_FLAGS)

$(TEST_OBJ)/%.o: $(TEST_SRC)/%.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@ $(TEST_LD_FLAGS)

cicd_run:
	$(error Thi is supposed to be run by CI/CD system. Run unit tests, and\
	generate docs if tests passed.)

# To assure it will work
.PHONY: clean
# Removes compiled files
clean:
	$(RM) -rf $(BINDIR)/* $(OBJ)/*

.PHONY: clean_test
clean_test:
	$(RM) -rf $(TEST_BINDIR)/* $(TEST_OBJ)/*

.PHONY: clean_all
clean_all: clean clean_test

# To assure it will work, whether docs directory is present or not
.PHONY: docs
docs:
	ifeq (, $(shell which doxygen))
	$(error No doxygen installed. Cannot generate docs.)
	endif
	$(RM) -rf docs
	doxygen doxyConfig

# Prints all .h and .c files
print: $(SRCS) $(wildcard $(SRC)/*.h) $(TEST_SRCS)
	ls -la $?