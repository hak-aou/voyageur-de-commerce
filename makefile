CC=gcc
CFLAG=-ansi -Wall
LDFLAGS=-lm -lncurses -lMLV
ALL=$(OBJ)main.o $(OBJ)permutation.o\
	$(OBJ)visite.o $(OBJ)outils.o

# .exe
EXE=pcv
BIN=./bin/
# .c
SRC=./src/
# .h
INC=./inc/
# .o
OBJ=./obj/

$(BIN)$(EXE): $(ALL)
	$(CC) -o $@ $^ $(CFLAG) $(LDFLAGS)

permutation.o: permutation.c permutation.h outils.h
visite.o: visite.c visite.h outils.h
main.o: main.c permutation.o visite.o

$(OBJ)%.o: $(SRC)%.c
	$(CC) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJ)*.o
	rm $(BIN)*