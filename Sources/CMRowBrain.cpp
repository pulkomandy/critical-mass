//	Critical Mass Be v. II
//	11/01/98
//	Hamish Carr

#include "CMRowBrain.h"

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
			theEvent->FindPointer("thinkBoard", &(void*)sentBoard);						//	retrieve the board
			theBoard = *sentBoard;											//	make a local copy
//			if (whichRow == 0)
//				{
//				printf("Received:\n");
//				theBoard.Print();
//				printf("======\n");
//				}
//			delete sentBoard;												//	and delete the dynamic one

			theEvent->FindInt32("player", &player);								//	retrieve the player
			theEvent->FindInt32("depth", &whatDepth);							//	retrieve the depth
			whichCol = 0;													//	start off searching col 0
			PostMessage(CM_MSG_CONSIDER_CELL);
			break;
		}
		default:
			BLooper::MessageReceived(theEvent);
		}
	}

void CMRowBrain::EvalCell()													//	evaluate the next cell for moves
	{
	long value;															//	the value of this cell
//	if (whichRow == 0)
//		{
//		printf("Entering EvalCell\n");
///		printf("Player = %d whichRow = %d whichCol = %d\n", player, whichRow, whichCol);
//		theBoard.Print();
//		printf("================\n");		
//		} 
		
	if (theBoard.IsLegalMove(whichRow, whichCol, player))							//	if it is a legal move
		{	
		testBoard = theBoard;												//	copy the board
//		if (whichRow == 0) {printf("The test board is:\n"); testBoard.Print();}
		testBoard.Move(player, whichRow, whichCol);								//	make the move
			if (testBoard.HasPlayerWon(player)) 								//	if that move wipes out the opponent
				value = PLAYER_WIPED_OUT * -player;							//	search no further
			else
				value = DeeperMinMax(-player, testBoard, whatDepth-1);				//	evaluate & store the move
		} // was a legal move
	else
		value = player * ILLEGAL_MOVE;										//	we can't play it
	BMessage *cellValueMessage = new BMessage(CM_MSG_CELL_RESULT);					//	create a new message
	cellValueMessage->AddInt32("cellValue", value);								//	add the value to it
	cellValueMessage->AddInt32("row", whichRow);									//	add the row to it
	cellValueMessage->AddInt32("column", whichCol);								//	add the column to it
//	if (whichRow == 0) printf("Reporting %d %d %d\n", whichRow, whichCol, value);
	master->PostMessage(cellValueMessage);										//	send it to our master
	whichCol++;															//	increment the column
	if (whichCol < theBoard.getWidth())											//	if there are more to search
		PostMessage(new BMessage(CM_MSG_CONSIDER_CELL));							//	tell ourselves to search another				
	} // end of EvalCell()
	
int CMRowBrain::DeeperMinMax(int thePlayer, CMBoard &theBoard, int depth)				//	minmax search
	{
	CMBoard testBoard;														//	used for testing moves
	int bestValue;															//	best value discovered so far
	int testValue;															//	value for test board
	int bestPossible = PLAYER_WIPED_OUT * -thePlayer;								//	best possible result:  wipeout
	
//	if (whichRow == 0) 
//		{
//		printf("In Deeper.  Player = %d Depth = %d\n", thePlayer, depth); theBoard.Print();
//		}
	
	bestValue = PLAYER_WIPED_OUT * thePlayer;									//	we hope to find something better than this		
	int i, j;																//	loop indices
	for (i = 0; i < theBoard.getHeight(); i++)										//	loop through rows
		for (j = 0; j < theBoard.getWidth(); j++)									//	and cells
			{
			if (theBoard.IsLegalMove(i, j, thePlayer))							//	if it's a legal move
				{
				testBoard = theBoard;										//	copy the board for testing
				testBoard.Move(thePlayer, i, j);								//	make the move
				if (testBoard.HasPlayerWon(thePlayer)) 							//	if that move wipes out the opponent
					testValue = PLAYER_WIPED_OUT * -thePlayer;					//	search no further
				else	if (depth > 1)
					testValue = DeeperMinMax(-thePlayer, testBoard, depth-1);		//	evaluate recursively
				else														//	base case
					testValue = testBoard.Evaluate();							//	just evaluate		
				} // end of legal move case
			else															//	otherwise
				testValue = ILLEGAL_MOVE * thePlayer;							//	say it's illegal (i.e. very bad)
			if (testValue == bestPossible) return bestPossible;					//	soon as we find one of these, leave
			if (thePlayer == RED_PLAYER)										//	i.e. we want negatives
				{
				if (testValue < bestValue) 									//	if it's better
					bestValue = testValue; 									//	keep it
				} // force the else clause
			else if (testValue > bestValue)									//	blue wants positive
				bestValue = testValue;										//	so keep it
			} // end of test loop
//	if (whichRow == 0) printf("Best Value is: %d\n", bestValue);
	return bestValue;														//	return the best value found
	} // end of DeeperMinMax()
	
