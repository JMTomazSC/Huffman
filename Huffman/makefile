CC = g++
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin

HUFF_OBJ = $(OBJ)/Huffman.o $(OBJ)/main.o
HUFF_INC = $(INC)/Huffman.hpp
ERROR_INC = $(INC)/Errors.hpp
CFLAGS = -Wall -c -I$(INC)

EXE = $(BIN)/huff

run: $(EXE)
	
$(BIN)/huff: $(HUFF_OBJ)
	$(CC) -pg -o $(BIN)/huff $(HUFF_OBJ) $(LIBS)

$(OBJ)/Huffman.o: $(HUFF_INC) $(SRC)/Huffman.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/Huffman.o $(SRC)/Huffman.cpp

$(OBJ)/main.o: $(Huffman_INC) $(ERROR_INC) $(SRC)/main.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/main.o $(SRC)/main.cpp

clean:
		rm -f $(EXE) $(HUFF_OBJ) gmon.out
		clear
