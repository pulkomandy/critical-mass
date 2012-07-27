//	Critical Mass Be
//	started 12/7/1997
//	Hamish Carr

#ifndef __CMGAMETURN_H__
#define __CMGAMETURN_H__

#include "CMBoard.h"

class CMGameTurn											//	class encapsulating game state
	{
	public:							
	CMBoard startBoard;										//	the board at the beginning of the turn
	CMBoard endBoard;										//	the board at the end of the turn
	long gameTurn;											//	which game turn it is
	long thePlayer;										//	which player moves this turn
	
	bool gameOver;											//	whether or not the game is over

	CMGameTurn();											//	default constructor
	CMGameTurn& operator =(const CMGameTurn &otherTurn);				//	copies a game turn
	void Advance();										//	creates successor turn
	void Print();											//	prints state to std out
	void MakeToroidal(bool isToroidal = true);					//	toggle toroidality
	}; // end of class CMGameTurn

#endif
