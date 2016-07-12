#ifndef PLAY_HPP
#define PLAY_HPP
#include "Chess.hpp"
//Change Turn
void changeTurn(int& turn)
{
    if (turn == 0) 
        turn = 1; 
    else if (turn == 1)
        turn = 0;
}


//Play Function
void playChess(Chess& game)
{
    string player[2]= {"White", "Black"};
    int turn = 0;
    char piece;
    int r,c,r1,c1;


    //Keep Playing until game is over
    while(game.gameOver() == false)
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
        changeTurn(turn);
    }
}

#endif 
