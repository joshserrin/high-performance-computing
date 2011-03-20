CC=mpicc
ROOMY_INSTALL=/home/jserrin/roomy-install
SRC=src
# -g : used for debugging.  If not required, remove it.
CFLAGS= -g -O0 -I$(ROOMY_INSTALL)/include -L$(ROOMY_INSTALL)/lib $(ROOMY_INSTALL)/lib/libroomy.a -lmpich -lpthread -lm

_DEPS=RoomyGraph.h
DEPS=$(patsubst %, $(SRC)/%, $(_DEPS))

_OBJ=RoomyGraph.o
OBJ=$(patsubst %, $(SRC)/%, $(_OBJ))

_TESTS=RoomyGraphTests.o
TESTS=$(patsubst %, $(SRC)/%, $(_TESTS))

# simple rule to check for .h modifications when compiling
# .c into .o files
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

compile: $(OBJ)

tests: compile $(TESTS)
	$(CC) -o RoomyGraphTest $(OBJ) $(TESTS) $(CFLAGS)

algs: compile src/GraphAlgorithmTest.c
	$(CC) -o GraphAlgorithmTest $(OBJ) src/GraphAlgorithmTest.c $(CFLAGS)

examples: compile src/SimpleExample.c
	$(CC) -o SimpleExample $(OBJ) src/SimpleExample.c $(CFLAGS)

refresh: clean tests examples algs

clean:
	rm -f $(SRC)/*.o
	rm -f RoomyGraphTest
	rm -f SimpleExample
	rm -f GraphAlgorithmTest
