//	Critical Mass Be
//	started 12/7/1997
//	Hamish Carr

#pragma once

#include "CMBoardQueue.h"

#include <stdlib.h>
#include <stdio.h>

CMBoardQueue::CMBoardQueue()									//	constructor
	{
	head = tail = NULL;										//	start with a null queue
	} // end of constructor

CMBoardQueue::~CMBoardQueue()									//	destructor
	{
	Reset();												//	empty the queue
	} // end of destructor
	
void CMBoardQueue::EnQueue(CMBoard newBoard)						//	queue up the board
	{
	CMBoardFrame *newFrame = new CMBoardFrame;					//	new frame for queueing
	newFrame->theBoard = newBoard;							//	copy the board into the frame
	newFrame->next = NULL;									//	null-terminate
	if (head == NULL)										//	if the queue is empty
		head = newFrame;									//	make the head point to it
	else													//	otherwise
		tail->next = newFrame;								//	connect it on to previous tail
	tail = newFrame;										//	make the tail point to it
	} // end of EnQueue()

CMBoard CMBoardQueue::DeQueue()								//	dequeue the board
	{
	CMBoard retBoard;										//	board to return (defaults to empty)
	CMBoardFrame *oldFrame;									//	the frame we are disposing of
	if (head == NULL) return retBoard;							//	return empty if queue empty
	retBoard = head->theBoard;								//	retrieve the board
	oldFrame = head;										//	grab the head
	head = head->next;										//	advance the head pointer
	if (head == NULL) tail = head;							//	unnecessary, but makes me happier
	delete oldFrame;										//	get rid of the old frame
	return retBoard;										//	return the stored board	
	} // end of DeQueue()
	
bool CMBoardQueue::Empty()									//	tests whether queue is empty
	{
	return (head == NULL);									//	simple	
	} // end of Empty();

void CMBoardQueue::Reset()									//	resets the queue to empty
	{
	CMBoardFrame *oldFrame;									//	the old frame
	
	for (; head != NULL; )									//	walk head down the queue
		{
		oldFrame = head;									//	grab the head
		head = head->next;									//	advance the head
		delete oldFrame;									//	get rid of the old one
		} // end of loop
	tail = head = NULL;										//	be paranoid & reset pointers
	} // end of Reset()

void CMBoardQueue::Print()									//	prints queue to std out
	{
	CMBoardFrame *step;										//	for walking down queue
	long i;												//	loop index
	for (step = head, i = 1; step != NULL; step = step->next, i++)	//	walk down queue
		{
		printf("Queue frame %d:\n", i);						//	print ID
		step->theBoard.Print();								//	print the board
		printf("\n");										//	print a CR
		} // end of walk down queue
	} // end of Print()
