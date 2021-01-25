//	Critical Mass Be v. II
//	11/01/98
//	Hamish Carr

#ifndef __CMROWBRAIN_H__
#define __CMROWBRAIN_H__

#include "CMBoard.h"
#include "CMGUIDefs.h"

#include <Looper.h>

class CMRowBrain : public BLooper								//	class for threaded thinking
	{
	private:
	int whichRow;											//	which row to consider
	BLooper *master;										//	who it reports to
	bool stoppedEarly;										//	whether we got stopped early
	int32 player;											//	which player we are searching for
	int whichCol;											//	which column we are considering
	int32 whatDepth;										//	what depth to search
	CMBoard theBoard;										//	the common board used for all the searches
	CMBoard testBoard;										//	kept globally to save construction
			
	public:
	CMRowBrain();											//	constructor
	~CMRowBrain();											//	destructor
	void SetMaster(BLooper *newMaster, int newRow);				//	sets the master
	virtual void MessageReceived(BMessage *theEvent);				//	responds to event messages

	void EvalCell();										//	evaluate the next cell for moves
	int DeeperMinMax(int thePlayer, CMBoard &theBoard, int depth);	//	minmax search
	}; // end of class CMRowBrain

#endif
