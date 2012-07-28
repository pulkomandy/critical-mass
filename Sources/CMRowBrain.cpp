//	Critical Mass Be v. II
//	11/01/98
//	Hamish Carr

#include "CMRowBrain.h"

#include <stdio.h>


CMRowBrain::CMRowBrain()														//	constructor
	: BLooper("Abelard")													//	call inherited constructor
{
	Run();																//	set the looper running
} // end of constructor 


void CMRowBrain::SetMaster(BLooper *newMaster, int newRow)							//	sets the master
{
	whichRow = newRow;														//	set the row number
	master = newMaster;														//	set it's master
	stoppedEarly = false;													//	set the flag
} // end of SetMaster()


CMRowBrain::~CMRowBrain()													//	destructor
{
} // end of destructor


void CMRowBrain::MessageReceived(BMessage *theEvent)								//	responds to event messages
{
	switch (theEvent->what)													//	big switch statement
	{
		case CM_MSG_ROW_STOP_THINKING:										//	command to cancel search
			stoppedEarly = true;											//	set flag to stop searching
			master->PostMessage(CM_MSG_ACK_ROW_STOP_THINKING);					//	acknowledge it
			break;
		case CM_MSG_CONSIDER_CELL:											//	command to search
			if (stoppedEarly) break;											//	flush useless searches
			EvalCell();													//	evaluate the next cell
			break;			
		case CM_MSG_CONSIDER_ROW:
		{
			stoppedEarly = false;											//	we haven't been stopped yet
			CMBoard *sentBoard = NULL;
			theEvent->FindPointer("thinkBoard", (void**)&sentBoard);
			theBoard = *sentBoard;
			delete sentBoard;

			theEvent->FindInt32("player", &player);
			theEvent->FindInt32("depth", &whatDepth);
			whichCol = 0;
			PostMessage(CM_MSG_CONSIDER_CELL);
			break;
		}
		default:
			BLooper::MessageReceived(theEvent);
	}
}

void CMRowBrain::EvalCell()		//	evaluate the next cell for moves
{
	long value;
//	if (whichRow == 0)
//		{
//		printf("Entering EvalCell\n");
//		printf("Player = %ld whichRow = %d whichCol = %d\n", player, whichRow, whichCol);
//		theBoard.Print();
//		printf("================\n");		
//		} 
		
	if (theBoard.IsLegalMove(whichRow, whichCol, player))
	{	
		testBoard = theBoard;
//		if (whichRow == 0) {printf("The test board is:\n"); testBoard.Print();}
		testBoard.Move(player, whichRow, whichCol);
		if (testBoard.HasPlayerWon(player))
			value = PLAYER_WIPED_OUT * -player;
		else
			value = DeeperMinMax(-player, testBoard, whatDepth-1);
	} else
		value = player * ILLEGAL_MOVE;

	BMessage *cellValueMessage = new BMessage(CM_MSG_CELL_RESULT);
	cellValueMessage->AddInt32("cellValue", value);
	cellValueMessage->AddInt32("row", whichRow);
	cellValueMessage->AddInt32("column", whichCol);
	/*if (whichRow == 0)*/ //printf("Reporting %d %d %ld\n", whichRow, whichCol, value);
	master->PostMessage(cellValueMessage);
	whichCol++;
	if (whichCol < theBoard.getWidth())
		PostMessage(new BMessage(CM_MSG_CONSIDER_CELL));
}
	
int CMRowBrain::DeeperMinMax(int thePlayer, CMBoard &theBoard, int depth)
{
	CMBoard testBoard;
	int bestValue = PLAYER_WIPED_OUT * thePlayer;
		//	we hope to find something better than this		
	int testValue;			//	value for test board
	int bestPossible = PLAYER_WIPED_OUT * -thePlayer;	//	best possible result:  wipeout
	
//	if (whichRow == 0) 
//		{
//		printf("In Deeper.  Player = %d Depth = %d\n", thePlayer, depth);
		//theBoard.Print();
//		}
	
	int i, j;
	for (i = 0; i < theBoard.getHeight(); i++)
		for (j = 0; j < theBoard.getWidth(); j++)
		{
			if (theBoard.IsLegalMove(i, j, thePlayer))
			{
				testBoard = theBoard;
				testBoard.Move(thePlayer, i, j);
				if (testBoard.HasPlayerWon(thePlayer))
					testValue = PLAYER_WIPED_OUT * -thePlayer;
				else if (depth > 1) {
//					if(whichRow == 0) printf("recurse %d %d %d\n", i, j, depth);
					testValue = DeeperMinMax(-thePlayer, testBoard, depth-1);
				} else {
//					if(whichRow == 0) printf("done recursing %d %d\n", i, j);
					testValue = testBoard.Evaluate();
				}
			} else
				testValue = ILLEGAL_MOVE * thePlayer;	//	say it's illegal (i.e. very bad)
			if (testValue == bestPossible) return bestPossible;	//	soon as we find one of these, leave
			if (thePlayer == RED_PLAYER)	//	i.e. we want negatives
			{
				if (testValue < bestValue)
					bestValue = testValue;
			} else if (testValue > bestValue)	//	blue wants positive
				bestValue = testValue;
		}
//	if (whichRow == 0) printf("Best Value is: %d\n", bestValue);
	return bestValue;
}

