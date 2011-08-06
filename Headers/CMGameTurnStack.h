//	Critical Mass Be
//	started 12/7/1997
//	Hamish Carr

#pragma once

#include "CMGameTurn.h"

class CMGameTurnFrame										//	frame on the stack
	{
	public:
	CMGameTurnFrame *next;									//	pointer to the next frame
	CMGameTurn theTurn;										//	the turn 
	}; // end of class CMGameTurnFrame
	
class CMGameTurnStack										//	class with a stack of game turns
	{
	private:
	CMGameTurnFrame *top;									//	pointer to the frames

	public:							
	CMGameTurnStack(); 										//	constructor
	~CMGameTurnStack();										//	destructor
	
	void Push(CMGameTurn pushee);								//	push board on stack
	CMGameTurn Pop();										//	pop board off stack
	bool Empty();											//	tests whether stack is empty
	void Reset();											//	resets the stack to empty
	
	void MakeToroidal(bool isToroidal = true);					//	resets all items in stack to tori
	void Print();											//	prints stack to std out
	}; // end of class CMGameTurnStack
