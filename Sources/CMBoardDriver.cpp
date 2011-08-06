//	CMBoard Test driver
//	7/12/1997
//	Hamish Carr

#include "CMBoard.h"

#include <stdlib.h>
#include <iostream>

#define RED_PLAYER -1
#define BLUE_PLAYER 1

main()
{
	CMBoard *theGameBoard = NULL;			//	the board in question
	CMBoard *theDuplicateBoard = NULL;		//	used for testing copy constructor
	CMBoard *theSwapBoard;					//	used for swapping boards
	char choice;							//	used for reading in choice
	int player = RED_PLAYER;				//	whose move it is
	int row;								//	for input
	int col;
	
	for (;;)
		{
		cout << endl << "Enter choice (m for menu): ";
		cin >> choice;
		cout << endl;
		
		switch (choice)
			{
			case 'm': case 'M':
				cout << "m/M  menu" << endl; 
				cout << "c    construct" << endl; 
				cout << "C    copy construct" << endl; 
				cout << "s/S  swap boards" << endl; 
				cout << "p/P  print board" << endl; 
				cout << "a/A  add bomb" << endl; 
				cout << "x/X  execute move" << endl;
				cout << "q/Q  quit" << endl;
				break;
			case 'c':
				if (theGameBoard != NULL)
					delete theGameBoard;
				theGameBoard = new CMBoard();
		//		cout << *theGameBoard;
				break;
			case 'C':
				if (theDuplicateBoard != NULL)
					delete theDuplicateBoard;
				theDuplicateBoard = theGameBoard;
				theGameBoard = new CMBoard(*theDuplicateBoard);
		//		cout << *theGameBoard;
				break;
			case 's': case 'S':
				theSwapBoard = theGameBoard;
				theGameBoard = theDuplicateBoard;
				theDuplicateBoard = theSwapBoard;
				break;
			case 'p': case 'P':
				if (theGameBoard == NULL)
					cout << "Null Game Board" << endl;
				else
	//				cout << *theGameBoard;
				break;
			case 'a': case 'A':
				cout << "Enter row & column (1-5, 1-6)" << endl;
				cin >> row >> col;
	//			if ((row < 1) || (row > N_ROWS) || (col < 1) || (col > N_COLS))
	//				cout << "Invalid row & column" << endl;
	//			else 
					{
					if (player == RED_PLAYER)
						{
						theGameBoard->AddRedBomb(row, col);
						cout << *theGameBoard;
						player = -player;
						}
					else 
						{
						theGameBoard->AddBlueBomb(row, col);
						cout << *theGameBoard;
						player = -player;
						}
					}
				break;
			case 'x': case 'X':
				cout << "Enter row & column (1-5, 1-6)" << endl;
				cin >> row >> col;
				if ((row < 1) || (row > N_ROWS) || (col < 1) || (col > N_COLS))
					cout << "Invalid row & column" << endl;
				else 
					{
					if (player == RED_PLAYER)
						{
						theGameBoard->MoveRed(row, col, true);
						player = -player;
						}
					else 
						{
						theGameBoard->MoveBlue(row, col, true);
						player = -player;
						}
					}
				break;
			case 'q': case 'Q':
				return 0;
			} // end of switch statement
		} // end of for loop
	} // end of main()

