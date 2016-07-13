#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;


class Board
{

//Data Members	
    char myboard[8][8];
	vector<char> capturedBlack;
	vector<char> capturedWhite;
	int king_loc[2][2];
//Member Functions -- Methods
public:
    Board(const string& = "input.txt");
    Board(const Board&); 
	bool isChecked(const int&);
	void move(const int&, const int&, const int&, const int&);
	bool legalMove(const int&, const int&, const int&, const int&, const int&);
	bool goodMoveHK(const int&, const int&, const int&, const int&, const int&);
	bool moveExists(const int&);
	void capture(const int&, const int&);

	char& operator()(const int&, const int&);
	Board& operator=(const Board&);

};



//Constructor -- from file
//************************************************** 
Board::Board(const string& in)
{
    ifstream reader(in);
    string line("");

	//If opening is successful
    if(reader.is_open())
    {
        for (int i = 0; i < 8; ++i) 
        {
            //Read a line
            getline(reader, line); 

            //Initialize each square of the board
            for (int j = 0; j < 8; ++j) 
            {
				if (line[j] == 'k')
				{
					king_loc[1][0] = i;
					king_loc[1][1] = j;
				}
				else if (line[j] == 'K')
				{
					king_loc[0][0] = i;
					king_loc[0][1] = j;
				}
                myboard[i][j] = line[j];
            }
        }
    }

	reader.close();
}



//Copy Constructor
//************************************************** 
Board::Board(const Board& b)
	:capturedBlack(b.capturedBlack), capturedWhite(b.capturedWhite)
{
	king_loc[0][0] = b.king_loc[0][0];
	king_loc[0][1] = b.king_loc[0][1];
	king_loc[1][0] = b.king_loc[1][0];
	king_loc[1][1] = b.king_loc[1][1];

	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			myboard[i][j] = b.myboard[i][j];
}



//Access Operator
//************************************************** 
char& Board::operator()(const int& r, const int& c)
{
	return myboard[r][c];
}




//Assignment Operator
//************************************************** 
Board& Board::operator=(const Board& b)
{
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			myboard[i][j] = b.myboard[i][j];

	return *this;
}


//IsChecked -- returns true if the player[turn]'s king is CHECKED
//************************************************** 
bool Board::isChecked(const int& turn)
{
	//King's Location
	int r1 = king_loc[turn][0];
	int c1 = king_loc[turn][1];
	
	//Enemy
	int enemy = (turn == 0) ? 1 : 0;

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			//If this piece is the enemy's
			if ((turn == 0 && islower(myboard[i][j])) || 
			    (turn == 1 && isupper(myboard[i][j])))
			{
				//If this piece can move to the king's location
				if (legalMove(enemy, i, j, r1, c1 ) == true)
					return true;
			}
		}
	}

	//If no piece can attack the King -- not checked
	return false;
}



//Move Function
//************************************************** 
void Board::move(const int& r, const int& c, const int& r1, const int& c1)
{
	myboard[r1][c1] = myboard[r][c];
	myboard[r][c] = '*'; 
}



//Capture Function
//************************************************** 
void Board::capture(const int& r, const int& c)
{
    if (isupper(myboard[r][c]))
        capturedWhite.push_back(myboard[r][c]);
    else
        capturedBlack.push_back(myboard[r][c]);

}



//Move Exists
//************************************************** 
bool Board::moveExists(const int& turn)
{
	//IMPLEMENT ME
	
	return true;
}


//Legal Move
//************************************************** 
bool Board::legalMove(const int& turn, const int& r, const int& c, const int& r1, const int& c1)
{
	//HouseKeeping -- boundary checking etc.
	//************************************************** 
	if (goodMoveHK(turn, r, c, r1, c1) == false)
	{
		cerr << "*** Invalid Move: Entry error!" << endl;
		return false;
	}
	

	char piece = myboard[r][c];	

    //Logic for each Piece
	//************************************************** 
    switch(piece)
    {
        //Pawn-Black
        case 'p':
        {
            //If blocked
            if (myboard[r1][c1] != '*')
            {
                //if successful capture 
                if (isupper(myboard[r1][c1]) && (r1 == r + 1) && (c1 == c - 1 || c1 == c + 1))                  
                { 
                    capture(r1, c1);
                    return true;

                }
                else
                    return false;
            }
            //If DOWN
            else if (c1 == c)
            {
                //If DOWN once
                if (r1 == r + 1)
                    return true;
                //If DOWN twice from initial position
                else if (r == 1 && r1 == r + 2)
                    return true;
                else
                    return false;
            }
            //Anything else
            else 
                return false;

            break;
        }//end-case 'p'


        //Pawn-White
        case 'P':
        {
            //If blocked
            if (myboard[r1][c1] != '*')
            {
                //if successful capture 
                if (islower(myboard[r1][c1]) && (r1 == r - 1) && (c1 == c - 1 || c1 == c + 1))                  
                { 
                    capture(r1, c1);
                    return true;
                }
                else
                    return false;
            }
            //If UP 
            else if (c1 == c)
            {
                //If up once
                if (r1 == r - 1)
                    return true;

                //If UP twice from initial
                else if (r == 6 && r1 == r - 2)
                    return true;

                else
					return false;
            }
            //Anything else
            else 
                return false;

            break;
        }


        //Rook
        case 'r':
        case 'R':
        {
            bool validPath = false;

            //Left-Right
            if (r1 == r)
            {
                //Left -- check path
                if (c1 < c)
                {
                    for(int i = c - 1; i > c1; --i)
                    {
                        if (myboard[i][c1] != '*')
                            return false;
                    }
                }
                //Right -- check path
                else if (c1 > c)
                {
                    for(int i = c + 1; i < c1; ++i)
                    {
                        if (myboard[i][c1] != '*')
                            return false;
                    }
                }
                validPath = true;

            }//end-LeftRight

            //Up-Down
            else if (c1 == c && r1 != r)
            {
                //Up
                if (r1 < r)
                {
                    for (int i = r - 1; i > r1; --i)
                    {
                        if (myboard[i][c1] != '*')
                            return false;
                    }
                }
                //Down
                else if (r1 > r)
                {
                    for (int i = r + 1; i < r1; ++i)
                    {
                        if (myboard[i][c1] != '*')
                            return false;
                    }
                }
                validPath = true;

            }//end-UpDown
            else
                return false;

            //Validate
            if (validPath && myboard[r1][c1] != '*')
            {
                if (islower(myboard[r][c]) && isupper(myboard[r1][c1]))
                {
                    capture(r1, c1);
                    return true;
                }
                else if (isupper(myboard[r][c]) && islower(myboard[r1][c1]))
                {
                    capture(r1, c1);
                    return true;
                }
                else
                    return false;
            }
            else if (validPath && myboard[r1][c1] == '*')
                return true;

            break;
        }//end case 'r' and 'R'


        //Knight
        case 'n':
        case 'N':
        {
            bool valid = false;

            //If player's own piece exists there
            if ((islower(piece) && islower(myboard[r1][c1])) ||
               (isupper(piece) && isupper(myboard[r1][c1])))
                return false;

            //Can move 2 up 1 LEFT or RIGHT 
            else if (r1 == r - 2 && (c1 == c - 1 || c1 == c + 1))
                valid = true;

            //Can move 2 right 1 UP or DOWN
            else if (c1 == c + 2 && (r1 == r - 1 || r1 == r + 1))
                valid = true;

            //Can move 2 down 1 LEFT or RIGHT
            else if (r1 == r + 2 && (c1 == c - 1 || c1 == c + 1))
                valid = true;

            //Can move 2 left 1 UP or DOWN
            else if (c1 == c - 2 && (r1 == r - 1 || r1 == r + 1))
                valid = true;

            else
                return false;
			
			//Validate
			if (valid == true)
			{
				if (myboard[r1][c1] == '*')
					return true;

				else if ((isupper(piece) && islower(myboard[r1][c1])) || 
						 (islower(piece) && isupper(myboard[r1][c1])))
				{
					capture(r1, c1);
					return true;
				}
			}
            break;
        }//end-case 'n' and 'N'
        

        //Bishop
        case 'b':
        case 'B':
        {
            bool validPath = false;
            int i, j;

            //UP-left 
            if (r1 < r && c1 < c)
            {
                i = r - 1;
                j = c - 1;

                //Validate path -- (path must be clear, diagonal)
                while (validPath == false)
                {
                    //If traversed past the target
                    if (i < r1 || j < c1)
                        return false;

                    else if (i == r1 && j == c1)
                        validPath = true;

                    //If a square along the path is blocked
                    else if (myboard[i][j] != '*')
                        return false;

                    //Iterate diagonally UP-Left
                    --i;
                    --j;
                }
            }//end-UpLeft
            //UP-right
            else if (r1 < r && c1 > c)
            {
                i = r - 1;
                j = c + 1;

                //Validate path -- (path must be clear, diagonal)
                while (validPath == false)
                {
                    //If traversed past the target
                    if (i < r1 || j > c1)
                        return false;

                    else if (i == r1 && j == c1)
                        validPath = true;

                    //If a square along the path is blocked
                    else if (myboard[i][j] != '*')
                        return false;

                    //Iterate diagonally UP-Right
                    --i;
                    ++j;
                }
            }//end-UpRight

            //DOWN-left
            else if (r1 > r && c1 < c)
            {
                i = r + 1;
                j = c - 1;

                //Validate path -- (path must be clear, diagonal)
                while (validPath == false)
                {
                    //If traversed past the target
                    if (i > r1 || j < c1)
                        return false;

                    else if (i == r1 && j == c1)
                        validPath = true;

                    //If a square along the path is blocked
                    else if (myboard[i][j] != '*')
                        return false;

                    //Iterate diagonally UP-Right
                    ++i;
                    --j;
                }
            }//end-DownLeft

            //DOWN-right
            else if (r1 > r && c1 > c)
            {
                i = r + 1;
                j = c + 1;

                //Validate path -- (path must be clear, diagonal)
                while (validPath == false)
                {
                    //If traversed past the target
                    if (i > r1 || j > c1)
                        return false;

                    else if (i == r1 && j == c1)
                        validPath = true;

                    //If a square along the path is blocked
                    else if (myboard[i][j] != '*')
                        return false;

                    //Iterate diagonally UP-Right
                    ++i;
                    ++j;
                }
            }//end-DownRight

            //Invalid Direction
            else
                return false;

            //If path is clear && it's diagonal
            if (validPath == true)
            {
                //If BLACK is moving
                if (islower(myboard[r][c]))
                {
                    //If square is taken by own piece
                    if (myboard[r1][c1] != '*' && islower(myboard[r1][c1]))
                        return false;

                    //If it's a CAPTURE
                    else if (isupper(myboard[r1][c1]))
                        capture(r1, c1);

                    return true;
                }
                //If WHITE is moving
                else
                {
                    //If square is taken by own piece
                    if (myboard[r1][c1] != '*' && isupper(myboard[r1][c1]))
                        return false;

                    //If it's a CAPTURE
                    else if (islower(myboard[r1][c1]))
                        capture(r1, c1);

                    return true;
                }
            }//end-ValidPath

            //Can move diagonally UP-right 
            //Can move diagonally DOWN-left or DOWN-right
            break;
        }//end-case 'b' and 'B'


        //King
        case 'k':
        case 'K':
        {
			bool validMove = false;

			//Left-Right
			if (r1 == r)
			{
				if (c1 == c - 1 || c1 ==  c + 1)
					validMove = true;
			}
			//Up Down
			else if (c1 == c)
			{
				if (r1 == r - 1 || r1 == r + 1)
					validMove = true;
			}
			//Diagonal-Once
			else if ((r1 == r + 1 || r1 == r - 1) && 
				     (c1 == c + 1 || c1 == c - 1))
				validMove = true;

			if (validMove == false)
				return false;

			//Empty square 
			if (myboard[r1][c1] == '*')
			{
				king_loc[turn][0] = r1;
				king_loc[turn][1] = c1;
				return true;
			}

			//If player's own piece exists there
			if ((islower(piece) && islower(myboard[r1][c1])) ||
			   (isupper(piece) && isupper(myboard[r1][c1])))
				return false;

			else if ((isupper(piece) && islower(myboard[r1][c1])) ||
			   (islower(piece) && isupper(myboard[r1][c1])))
			{
				capture(r1, c1);
				king_loc[turn][0] = r1;
				king_loc[turn][1] = c1;
				return true;
			}

			return false;	

            break;
        }//end-case 'K'


        //Queen
        case 'q':
        case 'Q':
        {
            bool validPath = false;
            int i,j;

            //Left -- Right
            if (r1 == r)
            {
                //Left -- check path
                if (c1 < c)
                {
                    for(i = c - 1; i > c1; --i)
                    {
                        if (myboard[i][c1] != '*')
                            return false;
                    }
                }
                //Right -- check path
                else if (c1 > c)
                {
                    for(i = c + 1; i < c1; ++i)
                    {
                        if (myboard[i][c1] != '*')
                            return false;
                    }
                }
                validPath = true;

            }//end-Left--Right

            //Up -- Down
            else if (c1 == c)
            {
                //Up
                if (r1 < r)
                {
                    for (i = r - 1; i > r1; --i)
                    {
                        if (myboard[i][c1] != '*')
                            return false;
                    }
                }
                //Down
                else if (r1 > r)
                {
                    for (i = r + 1; i < r1; ++i)
                    {
                        if (myboard[i][c1] != '*')
                            return false;
                    }
                }
                validPath = true;

            }//end-Up--Down

            //UP-left 
            else if (r1 < r && c1 < c)
            {
                i = r - 1;
                j = c - 1;

                //Validate path -- (path must be clear, diagonal)
                while (validPath == false)
                {
                    //If traversed past the target
                    if (i < r1 || j < c1)
                        return false;

                    else if (i == r1 && j == c1)
                        validPath = true;

                    //If a square along the path is blocked
                    else if (myboard[i][j] != '*')
                        return false;

                    //Iterate diagonally UP-Left
                    --i;
                    --j;
                }
            }//end-UpLeft

            //UP-right
            else if (r1 < r && c1 > c)
            {
                i = r - 1;
                j = c + 1;

                //Validate path -- (path must be clear, diagonal)
                while (validPath == false)
                {
                    //If traversed past the target
                    if (i < r1 || j > c1)
                        return false;

                    else if (i == r1 && j == c1)
                        validPath = true;

                    //If a square along the path is blocked
                    else if (myboard[i][j] != '*')
                        return false;

                    //Iterate diagonally UP-Right
                    --i;
                    ++j;
                }
            }//end-UpRight

            //DOWN-left
            else if (r1 > r && c1 < c)
            {
                i = r + 1;
                j = c - 1;

                //Validate path -- (path must be clear, diagonal)
                while (validPath == false)
                {
                    //If traversed past the target
                    if (i > r1 || j < c1)
                        return false;

                    else if (i == r1 && j == c1)
                        validPath = true;

                    //If a square along the path is blocked
                    else if (myboard[i][j] != '*')
                        return false;

                    //Iterate diagonally UP-Right
                    ++i;
                    --j;
                }
            }//end-DownLeft

            //DOWN-right
            else if (r1 > r && c1 > c)
            {
                i = r + 1;
                j = c + 1;

                //Validate path -- (path must be clear, diagonal)
                while (validPath == false)
                {
                    //If traversed past the target
                    if (i > r1 || j > c1)
                        return false;

                    else if (i == r1 && j == c1)
                        validPath = true;

                    //If a square along the path is blocked
                    else if (myboard[i][j] != '*')
                        return false;

                    //Iterate diagonally UP-Right
                    ++i;
                    ++j;
                }
            }//end-DownRight

            //Anything Else is INVALID
            else
                return false;
 
            //Validate
            if (validPath && myboard[r1][c1] != '*')
            {
                if (islower(myboard[r][c]) && isupper(myboard[r1][c1]))
                {
                    capture(r1, c1);
                    return true;
                }
                else if (isupper(myboard[r][c]) && islower(myboard[r1][c1]))
                {
                    capture(r1, c1);
                    return true;
                }
                else
                    return false;
            }
            else if (validPath && myboard[r1][c1] == '*')
                return true;

            break;
        }//end-case 'q' and 'Q'


        default:
            cerr << "*** Invalid Move: That piece does not exist! *** " << endl;
    }//end-Switch


	return true;

}



//Legal Move -- House Keeping
//************************************************** 
bool Board::goodMoveHK(const int& turn, const int& r, const int& c, const int& r1, const int& c1)
{
    char piece = myboard[r][c];

    //Validate if the piece is the player's
    if ((turn == 0 && islower(piece)) || (turn == 1 && isupper(piece)))
    {
        cerr << "*** You can move ONLY your OWN piece! ***" << endl;
        return false;
    }

    //Range check
    else if (r < 0 || r > 7 || r1 < 0 || r1 > 7 || c < 0 || c > 7 || c1 < 0 || c1 > 7)
    {
        cerr << "*** Invalid move: Out of range! ***" << endl;
        return false;
    }
    
    else if (r1 == r && c1 == c)
    {
        cerr << "*** Invalid Move: Cannot move to the current location! ***" << endl;
        return false;
    }
    else
        return true;
}


#endif
