//	Critical Mass Be
//	started 12/7/1997
//	Hamish Carr
#include "CMGameTurn.h"

#include <stdio.h>

CMGameTurn::CMGameTurn()											//	default constructor
	{
	gameTurn = 0;												//	set the game turn
	thePlayer = RED_PLAYER;										//	the player
	gameOver = false;											//	the game over flag
	} // end of CMGameTurn()
	
CMGameTurn &CMGameTurn::operator =(const CMGameTurn &otherTurn)				//	copies a game turn
	{
	gameTurn = otherTurn.gameTurn;								//	copy each item
	thePlayer = otherTurn.thePlayer;
	gameOver = otherTurn.gameOver;
	startBoard = otherTurn.startBoard;
	endBoard = otherTurn.endBoard;
	return *this;
	} // end of copy operator

void CMGameTurn::Advance()										//	creates successor turn
	{
	gameTurn++;												//	copy each item
	gameOver |= endBoard.HasPlayerWon(thePlayer);					//	update whether game is over
	thePlayer = -thePlayer;										//	switch players
	startBoard = endBoard;										//	copy the end state to the beginning
	} // end of Advance()	

void CMGameTurn::Print()											//	prints state to std out
	{
	printf("Game Turn %d\n", gameTurn);							//	just dump them one at a time
	printf("The Player is %d\n", thePlayer);
	printf("Start Board: \n");
	startBoard.Print();		
	printf("End Board: \n");
	endBoard.Print();		
	} // end of Print()
	
void CMGameTurn::MakeToroidal(bool isToroidal)						//	toggle toroidality
	{
	startBoard.MakeToroidal(isToroidal);							//	do boards individually
	endBoard.MakeToroidal(isToroidal);								//	do boards individually
	} // end of MakeToroidal()
