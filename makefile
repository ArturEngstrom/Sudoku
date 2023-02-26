.PHONY : all
all : sudoku 

sudoku : sudoku.cpp 
	g++ -o sudoku sudoku.cpp

	