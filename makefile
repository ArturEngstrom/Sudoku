.PHONY : all
all : sudoku 

sudoku : sudoku.cpp 
	g++ -o sudoku sudoku.cpp

# utils : board.hpp board.cpp
# 	g++ -o board board.cpp
	