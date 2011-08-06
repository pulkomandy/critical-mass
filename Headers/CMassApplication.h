//	CMassApplication.h

#pragma once	
#define SUICIDE_NOTE "Critical Mass Suicide Note\n\nAlas, poor me!\n\nSomething went wrong, and I must expiate my sins!\n\n"

#include "CMassWindow.h"
#include "CMassView.h"
#include "CMSound.h"
#include "CMGameTurnStack.h"
#include "CMBoardQueue.h"
#include "CMBrain.h"

#define N_SOUNDS 3
#define GAME_OVER_SOUND 0
#define BOOM_1_SOUND 1
#define BOOM_2_SOUND 2

class CMassApplication : public BApplication 									//	our application class
	{
	public:
	CMassApplication();															//	constructor

	private:
	CMassWindow *theCMWindow;													//	the window for the application
	CMSound *theSounds[N_SOUNDS];												//	the sounds to play
	BBitmap *theBitmaps[N_BITMAPS];												//	cache for the bitmaps
	CMBrain theCMBrain;															//	the brain that does all the thinking
	
	bool waitingForAckRejectClicks;												//	whether we are waiting for an ACK
	bool waitingForAckStopThinking;												//	whether we are waiting for this ACK
	bool waitingForComputerMove;												//	whether brain was already thinking
	bool sendDisplayEvents;														//	flag for whether to send display events
	bool doneDisplaying;														//	flag for display complete
	BMessage *pendingMove;														//	for hanging onto a pending move
	CMGameTurnStack theTurnStack;												//	the stack holding past turns
	CMGameTurn theTurn;															//	the current turn
	CMBoardQueue theDisplayQueue;												//	the display queue
	bigtime_t nextDisplayTime;													//	next time something will be displayed
	bigtime_t timeQuantum;														//	the time quantum between frames
	long theDisplayPlayer;														//	the player whose turn is being displayed
	bool showingExplosion;														//	flag for whether we are showing a boom frame
	bool soundOn;																//	flag for whether sound is on
	long redPlayerType, bluePlayerType;											//	which type(s) of players we have
	bool doingPulse;															//	flag for mutex in Pulse()
	bool isToroidal;															//	flag for toroidal mode
	bool isOpenGL;																//	flag for OpenGL mode

	void LoadSoundsAndImages();													//	function to load the sounds

	void ReactToIllegalMove();													//	what to do when given an illegal move
	void GenerateMove();														//	starts generating the next move
	void GenerateComputerMove();												//	same, but does nothing if human
	void HandleMove(BMessage *theEvent);										//	handle a move
	virtual void Pulse();														//	periodic processing
	void ResetGame();															//	reset to beginning of game
	void TakeBackMove();														//	reset to last human player (max 2 moves)
	void CancelThinking();														//	cancel any thinking
	void CancelDisplay();														//	cancel any display
	void ResetPlayer(int player, int playerType);								//	reset a particular player
		
	public:
	void FatalErrorAlert(char *message);										//	sets up an alert, then dies
	virtual void MessageReceived(BMessage *theEvent);							//	responds to event messages
	void ShowAlert(long whichItem);												//	shows an alert for rules, &c.
	}; // end of class CMassApplication

