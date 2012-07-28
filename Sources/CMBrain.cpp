//	Critical Mass Be v. II
//	11/01/98
//	Hamish Carr

#include "CMBrain.h"

#include <Application.h>

#include <assert.h>
#include <stdlib.h>

CMBrain::CMBrain()
	: BLooper("Rodin The Thinker")
{
	Run();
	rowBrains = NULL;
	nRows = 0;
	doingThreadedSearch = false;
	storedPriority = B_LOW_PRIORITY;
}


CMBrain::~CMBrain()
{
}


void CMBrain::MessageReceived(BMessage *theEvent)
{
	switch (theEvent->what)
	{
		case CM_MSG_STOP_THINKING:
			if (!doingThreadedSearch)
			{
				be_app->PostMessage(CM_MSG_ACK_STOP_THINKING);
				break;
			}
			for (long i = 0; i < nRows; i++)
			{
				set_thread_priority(rowBrains[i].Thread(), B_LOW_PRIORITY);
				rowBrains[i].PostMessage(CM_MSG_ROW_STOP_THINKING);
			}
			nDeadBrains = 0;
			break;
		case CM_MSG_ACK_ROW_STOP_THINKING: // From Row Brain
			nDeadBrains++;
			if (nDeadBrains == nRows)
			{
				set_thread_priority(Thread(), B_LOW_PRIORITY);
				be_app->PostMessage(CM_MSG_ACK_STOP_THINKING);
			}
			break;
		case CM_MSG_CELL_RESULT: // From Row Brain
		{
			long row, column, value;
			float percentDone;
			theEvent->FindInt32("row", &row); theEvent->FindInt32("column", &column);
			theEvent->FindInt32("cellValue", &value);
//			printf("%d %d %d\n", row, column, value);
			evalBoard.bombs[row][column] = value;
			nCellsSearched++;
			BMessage *updateMessage = new BMessage(CM_SHOW_PERCENT);
			percentDone = 1.0;
			updateMessage->AddFloat("percent", percentDone);
			be_app->PostMessage(updateMessage);
			if (nCellsSearched >= theBoard.getWidth()*theBoard.getHeight())
			{
				doingThreadedSearch = false;
				SelectRandomMove(whichPlayer);
			}
			break;
		}
		case CM_MSG_MAKE_MOVE:
		{
			long player, playerType;
			CMBoard *sentBoard = NULL;
			status_t errCode;
			errCode = theEvent->FindPointer("thinkBoard", (void**)&sentBoard);
			if (errCode != B_OK)
			{
				be_app->PostMessage(CM_MSG_CANT_MAKE_MOVE);
				break;
			}
			assert(sentBoard != NULL);
			theBoard = *sentBoard;	
			delete sentBoard;
			errCode = theEvent->FindInt32("player", &player);						//	retrieve the player
			if (errCode != B_NO_ERROR)										//	if it failed
				{
				be_app->PostMessage(CM_MSG_CANT_MAKE_MOVE);						//	complain
				break;
				} // end of no player
			errCode = theEvent->FindInt32("playerType", &playerType);				//	retrieve the player
			if (errCode != B_NO_ERROR)										//	if it failed
				{
				be_app->PostMessage(CM_MSG_CANT_MAKE_MOVE);						//	complain
				break;
				} // end of no player type
			doingThreadedSearch = false;										//	say it ain't so
			set_thread_priority(Thread(), storedPriority);						//	reset the priority
			switch (playerType)												//	depending on the player type
				{
				case ALBERT_EINSTEIN_PLAYER:									//	well, hell, I give
					EvalThreadedMinMax(player, 6);							//	call the threaded version
					break;
				case BILL_KOCAY_PLAYER:										//	pretty darn good
					EvalThreadedMinMax(player, 4);							//	call the threaded version
					break;
				case AI_PLAYER:											//	aspiring to village idiot status
					EvalMinMax(player, 2);									//	do a depth 2 search
					SelectRandomMove(player);								//	select a random result
					break;
				case SMART_PLAYER:											//	smarter than a hammer with a headache
					EvalValue(player);										//	evaluate each move
					SelectRandomMove(player);								//	and select a random one from evalBoard		
					break;
				case RANDOM_PLAYER:											//	smart as a sack of hammers
				default:													//	default to random behaviour
					EvalLegal(player);										//	evaluate which cells have legal moves
					SelectRandomMove(player);								//	and select a random one from evalBoard		
					break;
				} // end of switch for player type
		}
		case LowPriorityItem:												//	switch to low priority thought
			storedPriority = B_LOW_PRIORITY;									//	store the priority for later ;-)
			break;
		case NormalPriorityItem:												//	switch to normal priority thought
			storedPriority = B_NORMAL_PRIORITY;								//	store the priority for later ;-)
			break;
		case DisplayPriorityItem:											//	switch to display priority thought
			storedPriority = B_DISPLAY_PRIORITY;								//	store the priority for later ;-)
			break;
		case UrgentDisplayPriorityItem:										//	switch to urgent display priority thought
			storedPriority = B_URGENT_DISPLAY_PRIORITY;							//	store the priority for later ;-)
			break;
		case RealTimeDisplayPriorityItem:										//	switch to real time display priority thought
			storedPriority = B_REAL_TIME_DISPLAY_PRIORITY;						//	store the priority for later ;-)
			break;
		case UrgentPriorityItem:												//	switch to urgent priority thought
			storedPriority = B_URGENT_PRIORITY;								//	store the priority for later ;-)
			break;
		case RealTimePriorityItem:											//	switch to real time priority thought
			storedPriority = B_REAL_TIME_PRIORITY;								//	store the priority for later ;-)
			break;
		default:															//	if we don't recognize it
			BLooper::MessageReceived(theEvent);								//	call inherited routine
		} // end of switch
	} // end of MessageReceived()
	
void CMBrain::SelectRandomMove(int thePlayer)									//	chooses a random move from evalBoard
	{
	int i, j;																//	loop indices
	int nGoodMoves = 0;														//	how many equally good moves there are
	int randomNumber;														//	a random number for selecting a move
	double randFract;														//	a random number reduced to a fraction
	int bestMove = PLAYER_WIPED_OUT;											//	start with worst possible move
	int moveValue;															//	value of the given move

//	printf("Eval board is:\n");
//	evalBoard.Print();
	for (i = 0; i < theBoard.getHeight(); i++)										//	loop through rows
		for (j = 0; j < theBoard.getWidth(); j++)									//	and cells
			{
			moveValue = evalBoard.bombs[i][j] * thePlayer;						//	compute the cell's value
			if (moveValue > bestMove)										//	if it's better than the best so far 
				{
				nGoodMoves = 1;											//	reset the count
				bestMove = moveValue;										//	update the best value
				}
			else if (moveValue == bestMove)									//	if it's equal
				nGoodMoves++;												//	increment the count
			} // end of loop through cells (to find best value)

	randFract = rand();														//	get a random integer
	randFract /= RAND_MAX;													//	convert it to a float in [0,1]
	randomNumber = (int)(nGoodMoves * randFract + 1);

	for (i = 0; i < theBoard.getHeight(); i++)										//	loop through rows
		for (j = 0; j < theBoard.getWidth(); j++)									//	and cells
			{
			moveValue = evalBoard.bombs[i][j] * thePlayer;						//	compute the cell's value
			if (moveValue == bestMove)										//	if it is the right value
				{
				randomNumber--;											//	decrement the random
				if (randomNumber == 0)										//	if it's now 0, we're there
					{
					BMessage *theMessage = new BMessage(CM_MSG_MOVE_CHOSEN);		//	create a message for it
					theMessage->AddInt32("row", i); 							//	add the coordinates to the message
					theMessage->AddInt32("column", j);		
					be_app->PostMessage(theMessage);							//	send the message off
					delete theMessage;										//	get rid of the message when done
					set_thread_priority(Thread(), B_LOW_PRIORITY);				//	set the thread back to low priority
					return;												//	bail out of procedure
					} // end of case where right cell
				} // end of case where right value
			} // end of loop through cells
	} // end of SelectRandomMove()

void CMBrain::EvalLegal(int thePlayer)											//	evaluates the board for random brain
	{	
	int i, j;																//	loop indices
	evalBoard.Reset();														//	zero out evalBoard
	for (i = 0; i < theBoard.getHeight(); i++)										//	loop through rows
		for (j = 0; j < theBoard.getWidth(); j++)									//	and cells
			if (theBoard.IsLegalMove(i, j, thePlayer))							//	if it's a legal move
				evalBoard.bombs[i][j] = thePlayer;								//	say it's a good move
			else															//	otherwise
				evalBoard.bombs[i][j] = 0;									//	say it's not
	} // end of EvalLegal()
	
void CMBrain::EvalValue(int thePlayer)											//	evaluates the board for smart brain
	{
	int i, j;																//	loop indices
	evalBoard.Reset();														//	zero out evalBoard
	for (i = 0; i < theBoard.getHeight(); i++)										//	loop through rows
		for (j = 0; j < theBoard.getWidth(); j++)									//	and cells
			if (theBoard.IsLegalMove(i, j, thePlayer))							//	if it's a legal move
				{
				testBoard = theBoard;										//	copy the board for testing
				testBoard.Move(thePlayer, i, j);								//	make the move
				evalBoard.bombs[i][j] = testBoard.Evaluate();					//	evaluate & store the move
				}
			else															//	otherwise
				evalBoard.bombs[i][j] = ILLEGAL_MOVE * thePlayer;					//	say it's illegal (i.e. very bad)
	} // end of EvalValue()
	
void CMBrain::EvalMinMax(int thePlayer, int depth)								//	minmax search
	{
	CMBoard testBoard;														//	used for testing moves
	int i, j;																//	loop indices
	int testValue;															//	value for test board
	float percentDone;

	evalBoard.Reset();														//	zero out evalBoard
	for (i = 0; i < theBoard.getHeight(); i++)
		for (j = 0; j < theBoard.getWidth(); j++)
		{
			if (theBoard.IsLegalMove(i, j, thePlayer))							//	if it's a legal move
			{
				testBoard = theBoard;										//	copy the board for testing
				testBoard.Move(thePlayer, i, j);								//	make the move
				if (testBoard.HasPlayerWon(thePlayer)) 							//	if that move wipes out the opponent
					testValue = PLAYER_WIPED_OUT * -thePlayer;					//	search no further
				else
					testValue = DeeperMinMax(-thePlayer, testBoard, depth-1);		//	evaluate & store the move
			}
			else															//	otherwise
				testValue = ILLEGAL_MOVE * thePlayer;							//	say it's illegal (i.e. very bad)
			evalBoard.bombs[i][j] = testValue;									//	save the test value
			BMessage *updateMessage = new BMessage(CM_SHOW_PERCENT);				//	create a new message
			percentDone = 1.0; // One cell more
			updateMessage->AddFloat("percent", percentDone);						//	add the value to it
			be_app->PostMessage(updateMessage);								//	tell the app
//			printf("%d %d %d\n", i, j, testValue);
//			evalBoard.Print();
		} // end of loop through cells
//	printf("Result of evaluation\n");
//	evalBoard.Print();
	} // end of EvalMinMax()
	
int CMBrain::DeeperMinMax(int thePlayer, CMBoard &theBoard, int depth)				//	minmax search
	{
	CMBoard testBoard;														//	used for testing moves
	int bestValue;															//	best value discovered so far
	int testValue;															//	value for test board
	int bestPossible = PLAYER_WIPED_OUT * -thePlayer;								//	best possible result:  wipeout
	
	bestValue = PLAYER_WIPED_OUT * thePlayer;									//	we hope to find something better than this		
	int i, j;
	for (i = 0; i < theBoard.getHeight(); i++)
		for (j = 0; j < theBoard.getWidth(); j++)
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
	return bestValue;														//	return the best value found
	} // end of DeeperMinMax()
	
void CMBrain::EvalThreadedMinMax(long thePlayer, long depth)						//	minmax search
	{
	doingThreadedSearch = true;												//	set flag to say it's underway
	nCellsSearched = 0;														//	and this is how many we've searched so far
	whichPlayer = thePlayer;													//	save the player ID for later
	if (nRows != theBoard.getHeight())												//	we have the wrong # of brains
		{
		if (rowBrains != NULL) delete[] rowBrains;								//	get rid of any old copy
		rowBrains = new CMRowBrain[theBoard.getHeight()];								//	create new ones
		nRows = theBoard.getHeight();												//	reset the counter
		for (long i = 0; i < nRows; i++)										//	walk through them
			rowBrains[i].SetMaster(this, i);									//	set it's master		
	}
	for (long i = 0; i < nRows; i++)
	{
		CMBoard *sendBoard = new CMBoard(theBoard);
		assert (sendBoard != NULL);
		BMessage *thinkBoardMessage = new BMessage(CM_MSG_CONSIDER_ROW);
		status_t err = thinkBoardMessage->AddPointer("thinkBoard", sendBoard);
		assert(err == B_OK);
		thinkBoardMessage->AddInt32("player", thePlayer);
		thinkBoardMessage->AddInt32("depth", depth);
		rowBrains[i].PostMessage(thinkBoardMessage);
		delete thinkBoardMessage;
		set_thread_priority(rowBrains[i].Thread(), storedPriority);
	}
}
