#ifndef CHESS_HPP
#define CHESS_HPP

#include "Board.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

//Chess Class
//  Data Members:
//      -8x8 Chess board
//  Functions:
//      -Move(pieceName, int x, int y, int x1, int y1)
//      -CheckMove
//          * 
//      PrintBoard()


class Chess
{
    Board board;
    bool giveup;
public:
    Chess(const string& = "input.txt");            //Constructor
 

    void printBoard();      

    bool move(const int&, const char&, const int&, const char&, const int&);

    void promote(const int&, const int&);

    bool gameOver(const int&);

    bool exposesKing(const int&, const int&, const int&, const int&, const int&);
    bool checkMate();
    static int changeTurn(int);
private:
    int to_int_col(const char&);
    char to_char_col(const int&);
};



//Constructor
//************************************************** 
Chess::Chess(const string& filename)
    :board(filename), giveup(false)
{
}


//Print Board
//************************************************** 
void Chess::printBoard()
{
    cout << endl;
    cout << "    A B C D E F G H" << endl;
    cout << "   _________________" << endl;
    for (int i = 0; i < 8; ++i)
    {
        cout << i << "  |";
        for (int j = 0; j < 8; ++j)
        {
            if (board(i,j) == '*')
                cout << "-";
            else 
                cout << board(i,j);

            if(j != 7)
                cout << " ";
        }
        cout << "|";
        cout << endl;
    }
    cout << "   -----------------" << endl;
    cout << endl << endl;

    //If a KING is in Check
    if (board.isChecked(0) || board.isChecked(1))
    {
        cerr << "\tCHECK!!!" << endl;
    }
}



//Move Function
//************************************************** 
bool Chess::move(const int& turn, const char& r, const int& col, const char& r1, const int& col1)
{
    int c = to_int_col(col);
    int c1 = to_int_col(col1);

    //Give up
    if (r == -1)
    {
        giveup = true;
        return true;
    }

    //Determine if this move is LEGAL 
    if (board.legalMove(turn, r, c, r1, c1) == false)
    {
        cerr << "*** Invalid Move! ***" << endl;
        return false;
    }

    //Determine King will be EXPOSED
    if (exposesKing(turn, r, c, r1, c1) == true)
    {
        cerr << "*** Invalid Move: The King will be EXPOSED! ***" << endl;
        return false;
    }

    //Perform Move -- if everything is good
    //************************************************** 
    board.move(r, c, r1, c1);

    //Check if PROMOTION
    if (board(r1,c1) == 'p' && (r1 == 0 || r1 == 7))
        promote(r1, c1);


    return true;
}



//Promote
//************************************************** 
void Chess::promote(const int& r, const int& c)
{
    char p = '*';
    cout << "*** Promotion !!! ***" << endl;

    do
    {
        cout << "Select the PIECE to promote to (r,n,b,q): ";
        cin >> p;
    } while (p != 'r' && p != 'n' && p != 'b' && p != 'q');

    board(r,c) = p;
}



//Game Over Function
//************************************************** 
bool Chess::gameOver(const int& turn)
{
    if (giveup == true)
    {
        cout << "Match abandoned" << endl;
        return true;
    }

    //Game is not over if a legal move exists for player[turn]
    if (board.moveExists(turn) == true)
        return false;

    //CHECKMATE if player[turn] is in CHECK
    else if (board.isChecked(turn) == true)
    {
        printBoard();
        cout << endl << "CHECKMATE!" << endl;
    }
    else
    {
        printBoard();
        cout << endl << "STALEMATE" << endl;
    }

    cout << "Game Over" << endl;
    return true;
}



//Exposes King -- determine if this move leaves the King exposed
//************************************************** 
bool Chess::exposesKing(const int& turn, const int& r, const int& c, const int& r1, const int& c1)
{
    //Generate a board to make the pseudo-legal move
    Board pseudo_board(this->board);
    pseudo_board.move(r,c,r1,c1);

    //Check if the King can be attacked after this move
    if (pseudo_board.isChecked(turn) == true)
        return true;
    
    return false;
}


//Change Turn
//************************************************** 
int Chess::changeTurn(int turn)
{
    return (turn == 0) ? 1: 0;
}



//To Col Function
//************************************************** 
int Chess::to_int_col(const char& row)
{
    switch(row)
    {
        case 'a': 
        case 'A': 
            return 0;
            break;
        case 'b': 
        case 'B': 
            return 1;
            break;
        case 'c': 
        case 'C': 
            return 2;
            break;
        case 'd': 
        case 'D': 
            return 3;
            break;
        case 'e': 
        case 'E': 
            return 4;
            break;
        case 'f': 
        case 'F': 
            return 5;
            break;
        case 'g': 
        case 'G': 
            return 6;
            break;
        case 'h': 
        case 'H': 
            return 7;
            break;

        default:
            return 10;
    }
}



//To Col Function
//************************************************** 
char Chess::to_char_col(const int& r)
{
    switch(r)
    {
        case 0:
            return 'A';
            break;
        case 1:
            return 'B';
            break;
        case 2:
            return 'C';
            break;
        case 3:
            return 'D';
            break;
        case 4:
            return 'E';
            break;
        case 5:
            return 'F';
            break;
        case 6:
            return 'G';
            break;
        case 7:
            return 'H';
            break;
        default:
            return 'z';
    }
}



#endif
