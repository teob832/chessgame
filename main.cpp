#include "Chess.hpp"
#include <iostream>
using namespace std;

void playChess(Chess&);


int main()
{
    Chess game("input.txt");
    cout << "Welcome to Chess :)" << endl;

    playChess(game);


    return 0;
}


//Play Function
//*******************************************************************
void playChess(Chess& game)
{
    string player[2]= {"White", "Black"};
    int turn = 0;
    char piece;
    char r, r1;
    int c, c1;

    //Keep Playing until game is over -- CHECKMATE/STALEMATE
    while(game.gameOver(turn) == false)
    {
        game.printBoard();

        //Keep reading, until a successful move is made
        while (true)
        {
            cout << player[turn] << " enter move (current loc, loc to move): ";
            cin >> r >> c >> r1 >> c1; 

            if (game.move(turn, r, c, r1, c1) == true)
                break;
        }
        turn = Chess::changeTurn(turn);
    }
}

