CC=mpicc
ROOMY_INSTALL=/home/jserrin/roomy-install
SRC=src
CFLAGS= -I$(SRC)/ -I$(ROOMY_INSTALL)/include -L$(ROOMY_INSTALL)/lib $(ROOMY_INSTALL)/lib/libroomy.a -lmpich -lpthread -lm

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

compile_roomy_graph: $(OBJ)

tests: compile_roomy_graph $(TESTS) 
	$(CC) -o RoomyGraphTest $(OBJ) $(TESTS) $(CFLAGS)
	

clean:
	rm -f $(SRC)/*.o
