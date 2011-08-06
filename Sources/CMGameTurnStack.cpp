//	Critical Mass Be
//	started 12/7/1997
//	Hamish Carr

//	note:  implementation is not terribly efficient, but it's more reliable

#pragma once

#include "CMGameTurnStack.h"

#include <stdlib.h>
#include <stdio.h>

CMGameTurnStack::CMGameTurnStack()								//	constructor
	{
	top = NULL;											//	initialize to empty
	} // end of CMGameTurnStack()
	
CMGameTurnStack::~CMGameTurnStack()							//	destructor
	{
	Reset();												//	call the reset function
	} // end of destructor
		
void CMGameTurnStack::Push(CMGameTurn pushee)					//	push turn on stack
	{
//	printf("Pushing:\n");
//	pushee.Print();
	CMGameTurnFrame *newFrame = new CMGameTurnFrame;				//	create a new frame
	newFrame->theTurn = pushee;								//	copy the turn into it
	newFrame->next = top;									//	point to the top
	top = newFrame;										//	and make us the new top							
//	printf("Current Stack contents\n"); Print();
	} // end of Push()
	
CMGameTurn CMGameTurnStack::Pop()								//	pop board off stack
	{
	CMGameTurn returnTurn;									//	start off with an empty turn
//	printf("Initial value of returnTurn\n");
//	returnTurn.Print();
	if (Empty()) return returnTurn;							//	and return it if empty
	CMGameTurnFrame *oldFrame = top;							//	grab the top
	top = top->next;										//	move the top down
	returnTurn = oldFrame->theTurn;							//	copy the turn
//	printf("Final value of returnTurn\n");
//	returnTurn.Print();
	delete oldFrame;										//	delete the old frame
	return returnTurn;										//	and return the turn
	} // end of Pop()
	
bool CMGameTurnStack::Empty()									//	tests whether stack is empty
	{
	return (top == NULL);									//	simple, huh?
	} // end of empty()

void CMGameTurnStack::Print()									//	prints stack to std out
	{
	CMGameTurnFrame *step;									//	for walking down stack
	int i;												//	for counting them
	
	for (i = 1, step = top; step != NULL; i++, step = step->next)	//	walk down stack, counting
		{
		printf("Frame %d: \n", i);							//	print an ID
		step->theTurn.Print();								//	print the turn
		printf("\n");										//	print a CR
		} // end of loop	
	} // end of Print()
	
void CMGameTurnStack::Reset()									//	resets the stack to empty
	{
//	Print();
	while (!Empty()) Pop();									//	pop until you drop
	} // end of Reset()
	
void CMGameTurnStack::MakeToroidal(bool isToroidal)				//	resets all items in stack to tori
	{
	CMGameTurnFrame *step;									//	for walking down stack
	
	for (step = top; step != NULL; step = step->next)				//	walk down stack
		step->theTurn.MakeToroidal(isToroidal);					//	make the turn toroidal
	} // end of MakeToroidal
