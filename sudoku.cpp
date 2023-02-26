#include <iostream>
#include <fstream>
// #include "board.hpp"
#include <string>
#include <cmath>
#include <chrono>
#include <thread>


using namespace std;

struct Indexes{
    int irow;
    int icol;
};

struct Board{
    int **board;
    Indexes indexes[81];
};

bool containsCol(int **board, int c, int x);
bool containsRow(int **board, int r, int x);
bool containsBox(int **board, int r, int c, int x);
void showBoard(ifstream &file_sudoku, string line);
void showIndex(Board b);
Board readBoard(ifstream &file_sudoku, string line, Board b);
void solveBoard(Board b);
void freeMemory(int **board);
void showBoardRaw(Board b);
void showBoardClean(Board b);

int main(int argc, char **argv)
{
    ifstream file_sudoku("board.txt");
    string line;
    Board b;

    b = readBoard(file_sudoku, line, b);
    showBoardClean(b);
    solveBoard(b);
    showBoardClean(b);
    freeMemory(b.board);

    file_sudoku.close();
    return 0;
}

// Function definitions
void showBoard(ifstream& file_sudoku, string line){
    cout <<'\n';
    if (file_sudoku.is_open()){
        int i = 1;
        // Reset the read pointer to the beginning of the file
        file_sudoku.clear();
        file_sudoku.seekg(0, ios::beg);

        while (getline(file_sudoku, line))
        {
            string row = line;
            replace(row.begin(), row.end(), '_', ' ');
            string tmp0= row.substr(0,3);
            string tmp1= row.substr(3,3);
            string tmp2= row.substr(6,3);

            string new_row = tmp0 + "|" + tmp1 + "|" + tmp2;
            cout << new_row << '\n';
            if ((i%3==0) && (i!=9)){
                cout << "--- --- ---" << '\n';
            }
            i++;
        }
        cout <<'\n';
    }
}

Board readBoard(ifstream &file_sudoku, string line, Board b){
    b.board = new int *[9]; // create a pointer to an array of 9 pointers
    // initialize indexes
    for (int i = 0; i < 81; i++) {
        b.indexes[i].irow = -1;
        b.indexes[i].icol = -1;
    }

    if (file_sudoku.is_open())
    {
        int i = 0;
        int idx = 0;

        // Reset the read pointer to the beginning of the file
        file_sudoku.clear();
        file_sudoku.seekg(0, ios::beg);

        while (getline(file_sudoku, line))
        {   
            b.board[i] = new int[9]; // create a new array of 9 integers for each pointer
            string row = line;
            replace(row.begin(), row.end(), '_', '0');
            for (int j = 0; j < 9; j++){
                b.board[i][j] = (int)row[j] - '0'; // initialize each element and subtract ascii

                if (b.board[i][j] == 0){
                    b.indexes[idx].irow = i;
                    b.indexes[idx].icol = j;
                    idx++;
                }
            }
            i++;
        }
    }
    return b; // return the pointer to the matrix
}

void showIndex(Board b){
    for (int i = 0; i < 81;i++){
        cout << b.indexes[i].irow << "," << b.indexes[i].icol << '\n';
    }
}

void solveBoard(Board b){
    bool solved = false;
    int idx = 0; // 0-80
    int size = sizeof(b.indexes)/sizeof(b.indexes[0]); // 81
    while (!solved)
    {
        
        int irow = b.indexes[idx].irow;
        int icol = b.indexes[idx].icol;

        if (irow == -1){
            solved = true;
            break;
        }

        int val = b.board[irow][icol];
        // cout << "floor(irow)*3: " << floor(irow/3) * 3 << "   floor(icol)*3: " << floor(icol/3) * 3 << '\n';

        int newValue = val + 1;
        if (newValue > 9){ // reset board
            b.board[irow][icol] = 0;
            idx--;
            continue;
        }
        // cout << "irow: " << irow << "    icol: " << icol << "   val: " << val << "   newValue: " << newValue << "   idx: " << idx << '\n';


        if (!containsRow(b.board, irow, newValue) &
            !containsCol(b.board, icol, newValue) &
            !containsBox(b.board, floor(irow/3)*3, floor(icol/3)*3, newValue)){ // if allowed new value update board
            b.board[irow][icol] = newValue;
            idx++;
        } else {    // 
            b.board[irow][icol] = newValue;
            continue;
        }
        showBoardClean(b);
        // this_thread::sleep_for(chrono::microseconds(1000)); // if you want so slow down the print

        // Move cursor up 13 lines and clear the screen from the cursor position to the end of the screen
        cout << "\033[13A\033[0J";
    }
}

void freeMemory(int **board){
    // Free the memory used by the board
    for (int i = 0; i < 9; i++) {
        delete[] board[i];
    }
    delete[] board;
}

bool containsRow(int **board, int r, int x){
    for (int j = 0; j < 9; j++){
        if (board[r][j] == x){
            return true;
        }
    }
    return false;
}

bool containsCol(int **board, int c, int x){
    for (int i = 0; i < 9; i++){
        if (board[i][c] == x){
            return true;
        }
    }
    return false;
}

bool containsBox(int **board, int r, int c, int x) {
    for (int i = r; i < r+3; i++) {
        for (int j = c; j < c+3; j++) {
            if (board[i][j] == x) {
                return true;
            }
        }
    }
    return false;
}

void showBoardRaw(Board b){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (b.board[i][j] ==0){
                cout << " ";
            }
            else
            {
                cout << b.board[i][j];  
            }
            
        }
        cout << "\n";
    }
}

void showBoardClean(Board b){
    cout << "\n";
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (b.board[i][j] ==0){
                cout << " ";
                if (j==2 || j==5){
                    cout << "|";
                }
            }
            else
            {
                cout << b.board[i][j];  
                if (j==2 || j==5){
                    cout << "|";
                }
            }
        }
        if (i==2 || i==5){
            cout << "\n--- --- ---";
        }
        cout << "\n";
    }
    cout << "\n";
}