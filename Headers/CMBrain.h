//	Critical Mass Be v. II
//	11/01/98
//	Hamish Carr

#pragma once

#include "CMBoard.h"
#include "CMGUIDefs.h"
#include "CMRowBrain.h"

class CMBrain : public BLooper								//	class that does the thinking
	{
	private:
	CMBoard theBoard;										//	used for storing the board we were passed
	CMBoard testBoard;										//	used for testing cells
	CMBoard evalBoard;										//	used for evaluation results
	CMRowBrain *rowBrains;									//	array for the row brains
	bool doingThreadedSearch;								//	whether this search is threaded
	int nCellsSearched;										//	only used for threaded search
	int nRows;											//	# of current row brains
	int nDeadBrains;										//	count of dead brains
	int whichPlayer;										//	stored for use by threaded version
	long storedPriority;									//	priority stored for threaded searches
	
	public:
	CMBrain();											//	default constructor
	~CMBrain();											//	destructor
	virtual void MessageReceived(BMessage *theEvent);				//	responds to event messages
	
	private:
	void SelectRandomMove(int thePlayer);						//	chooses a random move from evalBoard
	void EvalLegal(int thePlayer);							//	evaluates the board for random brain
	void EvalValue(int thePlayer);							//	evaluates the board for smart brain
	void EvalMinMax(int thePlayer, int depth);					//	minmax search
	void EvalThreadedMinMax(long thePlayer, long depth);			//	minmax search
	int DeeperMinMax(int thePlayer, CMBoard &theBoard, int depth);	//	minmax search
	}; // end of class CMBrain
