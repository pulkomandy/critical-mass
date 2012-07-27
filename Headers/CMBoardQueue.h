//	Critical Mass Be
//	started 12/7/1997
//	Hamish Carr

#ifndef __CMBOARDQUEUE_H__
#define __CMBOARDQUEUE_H__

#include "CMBoard.h"

class CMBoardFrame											//	frame on the stack
	{
	public:
	CMBoardFrame *next;										//	pointer to the next frame
	CMBoard theBoard;										//	the turn 
	}; // end of class CMBoardFrame

class CMBoardQueue											//	class with a queue of boards
	{
	private:
	CMBoardFrame *head;										//	head of the queue
	CMBoardFrame *tail;										//	tail of the queue
	public:							
	CMBoardQueue(); 										//	constructor
	~CMBoardQueue();										//	destructor
	
	void EnQueue(CMBoard newBoard);							//	queue up the board
	CMBoard DeQueue();										//	dequeue the board
	bool Empty();											//	tests whether queue is empty
	void Reset();											//	empties out the queue

	void Print();											//	prints queue to std out
	}; // end of class CMBoardQueue

#endif
