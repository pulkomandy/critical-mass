//	CMassApplication.cpp

#include "CMassApplication.h"
#include <Resources.h>
#include <Alert.h>

char *soundNames[N_SOUNDS] = {"Da Winner", "Player 1 Boom", "Player 2 Boom"};

CMassApplication::CMassApplication()
		  		  : BApplication("application/x-critical-mass")							//	call inherited constructor
	{
	BRect aRect;																		//	for setting size of window
	LoadSoundsAndImages();																//	load the sounds
	aRect.Set(100, 100, 483, 470);														//	set a rectangle to a fixed size
	theCMWindow = new CMassWindow(aRect, theBitmaps);									//	and create the window
	theCMWindow->Show();																//	make the window visible
	
	waitingForAckRejectClicks = false;													//	we don't start off waiting
	waitingForAckStopThinking = false;													//	we don't start off waiting
	waitingForComputerMove = false;														//	flag for whether we thought ahead
	nextDisplayTime = system_time();													//	retrieve the time
	timeQuantum = MEDIUM_SPEED;															//	set the initial display speed
	pendingMove = NULL;																	//	we start with no move pending
	doingPulse = false;																	//	we don't start off in Pulse()
	SetPulseRate(100000);																//	tell it to pulse every 0.1 sec.
	theDisplayPlayer = RED_PLAYER;														//	set whose display it is
	redPlayerType = HUMAN_PLAYER; bluePlayerType = RANDOM_PLAYER;						//	set the player types												
	showingExplosion = false;															//	set the flag for explosions
	doneDisplaying = true;																//	say we're not done yet
	soundOn = true;																		//	switch sounds on
	isToroidal = false;																	//	we start off non-toroidal
	isOpenGL = false;																	//	and not rendering it
	GenerateMove();																		//	call routine to generate move
	} // end of CMassApplication constructor

void CMassApplication::LoadSoundsAndImages()											//	function to load the sounds
	{
	long i;																				//	loop index
	app_info CMAppInfo;																	//	used for finding where the application is
	BEntry CMEntry;																		//	entry in database representing the app
	BFile CMFile;																		//	the CMass application file
	BResources CMResources;																//	the CMass resources
	status_t errorCode;																	//	used for checking success
	char *aSound;																		//	a sound loaded
	size_t soundLength;																	//	length of the sound in bytes
	void *aPICT;																		//	a picture loaded - typed void b/c picHdl is a Mac type
	size_t pictLength;																	//	length of the pict in bytes
	BRect theRect;																		//	used for setting the size of the picts

	GetAppInfo(&CMAppInfo);																//	find out where on the disk the app is
	errorCode = CMEntry.SetTo(&CMAppInfo.ref);											//	convert this to a database entry
	if (errorCode != B_NO_ERROR)														//	if that didn't work
		FatalErrorAlert(DIE_BAD_ENTRY);													//	die gracefully
	errorCode = CMFile.SetTo(&CMEntry, B_READ_ONLY);									//	open it in read-only mode
	if (errorCode != B_NO_ERROR)														//	if that didn't work
		FatalErrorAlert(DIE_BAD_FILE);													//	die gracefully
	errorCode = CMResources.SetTo(&CMFile);												//	open the resources
	if (errorCode != B_NO_ERROR)														//	if that didn't work
		FatalErrorAlert(DIE_BAD_RESOURCES);												//	die gracefully
	for (i = 0; i < N_SOUNDS; i++)														//	walk through, reading in sounds
		{
		aSound = (char *) CMResources.FindResource(B_RAW_TYPE, i+1000, &soundLength);	//	retrieve the sound
		if (aSound != NULL)																//	i.e. it was successful
			theSounds[i] = new CMSound(soundNames[i], aSound, soundLength);				//	create the sound
		else																			//	oops, it doesn't exist
			FatalErrorAlert(DIE_BAD_SOUND);												//	die gracefully
		} // end of for i = 0 to N_SOUNDS
	theRect.Set(0, 0, CELL_SIZE-1, CELL_SIZE-1);										//	set dimensions of the PICT's
	for (i = 0; i < N_BITMAPS; i++)														//	walk through, reading in PICTs
		{
		theBitmaps[i] = NULL;															//	one at a time
		aPICT = CMResources.FindResource('PICT', i+1000, &pictLength);					//	retrieve the picture
		if (aPICT != NULL)																//	i.e. it was successful
			{
			theBitmaps[i] = new BBitmap(theRect, B_COLOR_8_BIT);						//	make a new bitmap with it
			theBitmaps[i]->SetBits(aPICT, pictLength, 0, B_COLOR_8_BIT);				//	and set the pixels
			free(aPICT);																//	free up the memory
			} // end of aPICT != NULL
		else																			//	oops, it doesn't exist
			FatalErrorAlert(DIE_BAD_PICT);												//	die gracefully
		} // end of for i = 0 to N_BITMAPS
	} // end of LoadSoundsAndImages()

void CMassApplication::FatalErrorAlert(char *message)									//	sets up an alert, then dies
	{
	BAlert *moribund;																	// declare an alert
		
	moribund = new BAlert("Critical Mass Suicide Note", message, "OK.  Go ahead and die!", NULL, NULL, 
							B_WIDTH_FROM_LABEL,B_STOP_ALERT);							//	set the alert up
	moribund->Go();																		//	run it & wait for it to return
	Quit();																				//	then crash out of the application
	} // end of FatalErrorAlert()

void CMassApplication::MessageReceived(BMessage *theEvent)								//	responds to event messages
	{
	switch (theEvent->what)																//	just a big switch statement
		{
		case CM_MSG_MOVE_CHOSEN:														//	message from view to app with move
			if (!waitingForAckRejectClicks && !waitingForAckStopThinking)				//	if there's nothing we're waiting for
				if (doneDisplaying)														//	if display of last move is done
					HandleMove(theEvent);												//	call subroutine
				else																	//	i.e. display is not done yet 
					{
					pendingMove = new BMessage(*theEvent);								//	otherwise, store a copy of it
					waitingForComputerMove = false;										//	we're no longer waiting
					} // end of case where move is pending
			break;
			
		case CM_MSG_ACK_REJECT_CLICKS:													//	view acknowledges "reject" message
			waitingForAckRejectClicks = false;											//	set flag to say no longer waiting
//			printf("rejection is over\n");
			if (waitingForAckStopThinking)												//	check whether we are waiting for this too
				break;
			GenerateMove();																//	start thinking up a move
			break;

		case CM_MSG_ACK_STOP_THINKING:													//	brain acknowledges "stop" message
			waitingForAckStopThinking = false;											//	set flag to say no longer waiting
//			printf("thinking is over\n");
			if (waitingForAckRejectClicks)												//	check whether we are waiting for this too
				break;
			GenerateMove();																//	start thinking up a move
			break;
	
		case CM_MSG_CANT_MAKE_MOVE:														//	brain couldn't understand our message
			waitingForComputerMove = false;												//	turn off this flag
			if (waitingForAckStopThinking || waitingForAckRejectClicks)					//	if we're already cancelling stuff
				break;
			GenerateMove();																//	otherwise, start getting a move
			break;

		case CM_MSG_DISPLAY_DONE:														//	we've finished displaying
			doneDisplaying = true;														//	set flag saying we're done
			if (waitingForAckStopThinking || waitingForAckRejectClicks)					//	if we're already cancelling stuff
				break;
			if (waitingForComputerMove)													//	if we're already waiting
				break;
			if (pendingMove != NULL)													//	if there is a move already pending
				{
				PostMessage(pendingMove);												//	post the pending move to the queue
				delete pendingMove;														//	delete the message
				pendingMove = NULL;														//	and reset the pointer
				} // end of case where move pending
			else
				GenerateMove();															//	otherwise, start getting a move
			break;
			
		case CM_SHOW_PERCENT:															//	if the message is to update percent done
		{
			BMessage *repeater = DetachCurrentMessage();								//	copy the message
			theCMWindow->PostMessage(repeater);											//	send it to the window
			break;
		}
		case AboutCMItem:																//	standard about box	
			ShowAlert(AboutCMItem);														//	show the alert box
			break;
		case NewGameItem:																//	restart game
			ResetGame();																//	call the reset game function
			break;
		case TakeBackItem:																//	take back last move
			TakeBackMove();																//	call routine to do it
			break;
		case QuitItem:																	//	quit the game
			QuitRequested();															//	call the standard quit requested function
			break;
			
		case Player1HumanItem:															//	define the red player to be human
		case Player1RandomItem:															//	define the red player to be random
		case Player1SmartItem:															//	define the red player to be smart
		case Player1AIItem:																//	define the red player to be AI
		case Player1IsBillKocayItem:													//	define the red player to be Bill Kocay
		case Player1IsAlbertEinsteinItem:												//	define the red player to be Albert Einstein
			if ((theTurn.thePlayer == RED_PLAYER) && 									//	if it's the active turn
				((waitingForAckRejectClicks) || (waitingForAckStopThinking)))			//	and we are already cancelling
				{
				PostMessage(theEvent->what);											//	repost the message
				snooze(10000);															//	snooze for a while
				break;
				} // end of repetitive choice
			ResetPlayer(RED_PLAYER, theEvent->what - Player1HumanItem);					//	set the type
			theCMWindow->PostMessage(new BMessage(theEvent->what));						//	tell window to update menu
			break;
		case Player2HumanItem:															//	define the blue player to be human
		case Player2RandomItem:															//	define the blue player to be random
		case Player2SmartItem:															//	define the blue player to be smart
		case Player2AIItem:																//	define the blue player to be AI
		case Player2IsBillKocayItem:													//	define the blue player to be Bill Kocay
		case Player2IsAlbertEinsteinItem:												//	define the blue player to be Albert Einstein
			if ((theTurn.thePlayer == BLUE_PLAYER) && 									//	if it's the active turn
				((waitingForAckRejectClicks) || (waitingForAckStopThinking)))			//	and we are already cancelling
				{
				PostMessage(theEvent->what);											//	repost the message
				snooze(10000);															//	snooze for a while
				break;
				} // end of repetitive choice
			ResetPlayer(BLUE_PLAYER, theEvent->what - Player2HumanItem);				//	set the type
			theCMWindow->PostMessage(new BMessage(theEvent->what));						//	tell window to update menu
			break;
		case OptionFastDisplayItem:														//	fast display mode
			timeQuantum = FAST_SPEED;													//	set the time quantum
			theCMWindow->PostMessage(new BMessage(OptionFastDisplayItem));				//	and tell the window to revise the menus
			break;
		case OptionMediumDisplayItem:													//	medium display mode
			timeQuantum = MEDIUM_SPEED;													//	set the time quantum
			theCMWindow->PostMessage(new BMessage(OptionMediumDisplayItem));			//	and tell the window to revise the menus
			break;
		case OptionSlowDisplayItem:														//	slow display mode
			timeQuantum = SLOW_SPEED;													//	set the time quantum
			theCMWindow->PostMessage(new BMessage(OptionSlowDisplayItem));				//	and tell the window to revise the menus
			break;
		case OptionSoundToggleItem:														//	toggle sound on/off
			soundOn = !soundOn;															//	toggle the flag
			theCMWindow->PostMessage(new BMessage(soundOn ? CM_MARK_SOUND_ON : CM_MARK_SOUND_OFF));
			break;
		case OptionWrapAroundItem:														//	toggle toroidality
			CancelThinking();															//	cancel thinking the usual way
			isToroidal = !isToroidal;													//	reverse toroidality
			theTurnStack.MakeToroidal(isToroidal);										//	apply to the stack
			theTurn.MakeToroidal(isToroidal);											//	and to the turn
			theCMWindow->PostMessage(new BMessage(OptionWrapAroundItem));				//	revise the menus
			if ((!isToroidal) && isOpenGL)												//	if it was rendered, and we unwrapped
				{
				isOpenGL = !isOpenGL;													//	flip the toggle
				theCMWindow->PostMessage(new BMessage(OptionRenderTorusItem));			//	switch off the render option
				} // end of special case
			break;
		case OptionRenderTorusItem:														//	toggle GL view
			theCMWindow->PostMessage(new BMessage(OptionRenderTorusItem));				//	tell the view to do the same
			break;					
		case HelpGameRulesItem:															//	show game rules
			ShowAlert(HelpGameRulesItem);												//	show the alert box
			break;
		case HelpMenuOptionsItem:														//	show help on menu options
			ShowAlert(HelpMenuOptionsItem);												//	show the alert box
			break;
		case LowPriorityItem:															//	switch to low priority thought
		case NormalPriorityItem:														//	switch to normal priority thought
		case DisplayPriorityItem:														//	switch to display priority thought
		case UrgentDisplayPriorityItem:													//	switch to urgent display priority thought
			theCMBrain.PostMessage(theEvent->what);										//	tell the brain about it
			theCMWindow->PostMessage(theEvent->what);									//	and the window
			break;
		case RealTimeDisplayPriorityItem:												//	switch to real time display priority thought
		case UrgentPriorityItem:														//	switch to urgent priority thought
		case RealTimePriorityItem:														//	switch to real time priority thought
		{
			BAlert *alert;																//	the alert that we will use
			long result;																//	to retrieve the result
			alert = new BAlert("Processor Hogging", "Are you sure that you want to give the brain such a disgustingly high priority?\n\nThis will seriously screw up other applications", "Yes", "No", NULL, B_WIDTH_AS_USUAL);
			BRect windowRect = theCMWindow->Frame();									//	find the screen coords of the window
			BRect alertRect = alert->Frame();											//	find the frame for the alert
			alertRect.left = windowRect.left + ((windowRect.right - windowRect.left) - (alertRect.right - alertRect.left))/2;
			alertRect.top = windowRect.top + ((windowRect.bottom - windowRect.top) - (alertRect.bottom - alertRect.top))/2;
																						//	centre the alert in the view
			alert->MoveTo(alertRect.left, alertRect.top);								//	move the alert correspondingly
			result = alert->Go();														//	run the alert box
			if (result == 1) break;														//	if the answer is no, don't do it
			theCMBrain.PostMessage(theEvent->what);										//	tell the brain about it
			theCMWindow->PostMessage(theEvent->what);									//	and the window
			break;
		}
		default:																		//	if we don't recognize it
			BApplication::MessageReceived(theEvent);									//	pass it on to inherited method
		} // end of switch on theEvent->what
	} // end of MessageReceived()
	
void CMassApplication::ReactToIllegalMove()												//	what to do when given an illegal move
	{
	BMessage acceptClickMessage(CM_MSG_ACCEPT_CLICKS);									//	create a message to send
	theCMWindow->PostMessage(&acceptClickMessage);										//	post the message
	} // end of ReactToIllegalMove()

void CMassApplication::ShowAlert(long whichItem)										//	shows an alert for rules, &c.
	{
	BAlert *alert;																		//	the alert that we will use
	long whichAlert;																	//	which box to put up
	long result;																		//	to retrieve the result

	switch(whichItem)																	//	convert to 0..3 index
		{
		case AboutCMItem:	
			whichAlert = 0; 
			break;
		case HelpGameRulesItem:
			whichAlert = 1;
			break;
		case HelpMenuOptionsItem:
			whichAlert = 2;
			break;
		} // end of switch(whichItem
		
	while (whichAlert != 3)																//	keep going until they hit done
		switch (whichAlert)																//	switch on which alert to put up
			{
			case 0:																		//	About box
				alert = new BAlert("About Critical Mass", HELP_STRING, "Rules", "Menus", "Done", B_WIDTH_AS_USUAL);
				result = alert->Go();													//	run the alert box
				whichAlert = result + 1;												//	compensate for not having "about" button
				break;
			case 1:																		//	Rules box
				alert = new BAlert("Critical Mass Rules", RULES_STRING, "About", "Menus", "Done", B_WIDTH_AS_USUAL);
				result = alert->Go();													//	run the alert box
				if (result > 0)
					whichAlert = result + 1;											//	compensate for not having "rules" button
				else whichAlert = result;
				break;
			case 2:																		//	Menu box
				alert = new BAlert("About Critical Mass", MENU_STRING, "About", "Rules", "Done", B_WIDTH_AS_USUAL);
				result = alert->Go();													//	run the alert box
				if (result > 1)
					whichAlert = result + 1;											//	compensate for not having "menu" button
				else whichAlert = result;
				break;
			} // end of switch(whichItem)
		
	} // end of ShowAlert()

void CMassApplication::HandleMove(BMessage *theEvent)									//	handle a move
	{
	if (waitingForAckRejectClicks)														//	if we have already cancelled moves
		return;
	long row, column;																	//	the row and column chosen
	status_t errCode = theEvent->FindInt32("row", &row);								//	retrieve the row
	if (errCode != B_NO_ERROR)															//	if it failed
		{
		ReactToIllegalMove();															//	do whatever it is we do
		return;
		} // end of failure to find row
	errCode = theEvent->FindInt32("column", &column);									//	retrieve the column
	if (errCode != B_NO_ERROR)															//	if it failed
		{
		ReactToIllegalMove();															//	do whatever it is we do
		return;
		} // end of failure to find column
	if (!theTurn.endBoard.IsLegalMove(row, column, theTurn.thePlayer))					//	if it is not a legal move
		{
		ReactToIllegalMove();															//	do whatever it is we do
		return;
		} // end of failure to find column
	theTurn.endBoard.AddBomb(theTurn.thePlayer, row, column);							//	add the bomb
	theDisplayQueue.EnQueue(theTurn.endBoard);											//	and add it to display queue
	bool explosionsExist;																//	flag for whether explosions happen
	for (;;)																			//	loop forever
		{
		if (theTurn.endBoard.HasPlayerWon(theTurn.thePlayer)) break;					//	if the player has won, leave now
		explosionsExist = theTurn.endBoard.StackExplosions();							//	call routine to create stack
		if (!explosionsExist)															//	if there are no explosions to do
			break;																		//	leave the loop
		theDisplayQueue.EnQueue(theTurn.endBoard);										//	add the explosion board to queue
		theTurn.endBoard.Explode(theTurn.thePlayer);									//	do a round of explosions
		theDisplayQueue.EnQueue(theTurn.endBoard);										//	add the result board to queue
		} // end of outer for loop
	theTurnStack.Push(theTurn);															//	push the turn onto the stack
	showingExplosion = false;															//	set flag saying not showing booms
	theDisplayPlayer = theTurn.thePlayer;												//	set the player whose move is being shown
	sendDisplayEvents = true;															//	set flag to allow display
	doneDisplaying = false;																//	say we're not done yet
	theTurn.Advance();																	//	move to the next turn, already
	theCMWindow->PostMessage(CM_RESET_STATUS_BAR);										//	reset the status bar
	waitingForComputerMove = false;														//	reset the flag
	GenerateComputerMove();																//	start thinking up a move
	} // end of HandleMove()

void CMassApplication::Pulse()															//	periodic processing
	{
	bigtime_t now;																		//	the current time
	if (!sendDisplayEvents) return;														//	bail out if not allowed to display
	doingPulse = true;																	//	set flag to lock out others
	if (!theDisplayQueue.Empty())														//	bail out if no events
		{
		if ((now = system_time()) >= nextDisplayTime)									//	if it's time to send another frame
			{
			if (showingExplosion && soundOn)											//	if this frame is a boom with sound

				if (theDisplayPlayer == RED_PLAYER)										//	depending on which player
					theSounds[BOOM_1_SOUND]->Play();									//	play a boom sound
				else
					theSounds[BOOM_2_SOUND]->Play();									//	play a different boom sound

			showingExplosion = !showingExplosion;										//	next frame isn't
			nextDisplayTime = now + timeQuantum;										//	set when next display occurs
			CMBoard nextBoard; nextBoard = theDisplayQueue.DeQueue();								//	retrieve the board
			CMBoard *sendBoard = new CMBoard(nextBoard);								//	set the board to be sent
			BMessage *showBoardMessage = new BMessage(CM_MSG_SHOW_BOARD);				//	create a message to send
			showBoardMessage->AddPointer("displayBoard", sendBoard);					//	view is responsible for deleting the board
			theCMWindow->PostMessage(showBoardMessage);									//	post the message
			delete showBoardMessage;													//	and delete it when done
			} // end of case where frame displayed	
		if (theDisplayQueue.Empty())													//	if the display queue is now empty
			{
			if (!theTurn.gameOver)														//	if the game isn't over yet
				be_app->PostMessage(CM_MSG_DISPLAY_DONE);								//	tell the looper the display sequence is done
			else																		//	i.e. game is over
				{

				if (soundOn)															//	if sound is switched on
					theSounds[GAME_OVER_SOUND]->Play();									//	play a big sound

				BAlert *victoryAlert;													// 	declare an alert
				victoryAlert = new BAlert("Critical Mass Victory", 
										theDisplayPlayer == RED_PLAYER ? 	"Red Player Won!\n\nChoose New Game to restart" :
																				"Blue Player Won!\n\nChoose New Game to restart"
									, "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);//	set the alert up
				BRect windowRect = theCMWindow->Frame();								//	find the screen coords of the window
				BRect alertRect = victoryAlert->Frame();								//	find the frame for the alert
				alertRect.left 	= windowRect.left + ((windowRect.right - windowRect.left)
								- (alertRect.right - alertRect.left))/2;
				alertRect.top 	= windowRect.top + ((windowRect.bottom - windowRect.top) 
								- (alertRect.bottom - alertRect.top))/2;				//	centre the alert in the view
				victoryAlert->MoveTo(alertRect.left, alertRect.top);					//	move the alert correspondingly
				victoryAlert->Go();														//	run it & wait for it to return
				} // end of game over case
			} // end of case where display queue now empty
		} // end of case where move to display
	doingPulse = false;																	//	switch flag off
	} // end of Pulse()
	
void CMassApplication::GenerateMove()													//	starts generating the next move
	{
	long playerType = (theTurn.thePlayer == RED_PLAYER) ? redPlayerType : bluePlayerType;
//	printf("Starting to think. . . \n");
	switch (playerType)																	//	depending on the player type
		{	
		case RANDOM_PLAYER:																//	smart as a sack of hammers
		case SMART_PLAYER:																//	smarter than a hammer with a headache
		case AI_PLAYER:																	//	aspiring to village idiot status
		case BILL_KOCAY_PLAYER:															//	pretty darn good
		case ALBERT_EINSTEIN_PLAYER:													//	well, hell, I give
		{
			CMBoard *sendBoard = new CMBoard(theTurn.startBoard);						//	set the board to be sent
			BMessage *thinkBoardMessage = new BMessage(CM_MSG_MAKE_MOVE);				//	create a message to send
			thinkBoardMessage->AddPointer("thinkBoard", sendBoard);						//	brain is responsible for deleting the board
			thinkBoardMessage->AddInt32("player", theTurn.thePlayer);					//	add the player's ID
			thinkBoardMessage->AddInt32("playerType", playerType);						//	add the player's type
			theCMBrain.PostMessage(thinkBoardMessage);									//	post the message
			delete thinkBoardMessage;													//	and delete it when done
			break;
		}
		case HUMAN_PLAYER:																//	ooh, a real live person
			theCMWindow->PostMessage(new BMessage(CM_MSG_ACCEPT_CLICKS));				//	tell the view that clicks are good
			break;
		} // end of switch
	} // end of GenerateMove()

void CMassApplication::GenerateComputerMove()											//	same, but does nothing if human
	{
	long playerType = (theTurn.thePlayer == RED_PLAYER) ? redPlayerType : bluePlayerType;
	if (playerType != HUMAN_PLAYER)														//	if it's not human
		{
		theCMWindow->PostMessage(new BMessage(CM_RESET_STATUS_BAR));					//	reset the status bar
		theCMWindow->PostMessage(new BMessage((theTurn.thePlayer == RED_PLAYER) ?		//	change the status bar's colour
				CM_MAKE_STATUS_BAR_RED : CM_MAKE_STATUS_BAR_BLUE));
		waitingForComputerMove = true;													//	set flag saying we have started
		GenerateMove();																	//	call the regular mechanism
		} // end of case where not human
	} // end of GenerateComputerMove()
	
void CMassApplication::ResetGame()														//	reset to beginning of game
	{
	if ((waitingForAckRejectClicks) || (waitingForAckStopThinking))						//	if we are already cancelling
		{
		PostMessage(NewGameItem);														//	repost the message
		snooze(10000);																	//	snooze for a while
		return;
		} // end of repetitive choice
	CancelThinking();																	//	cancel any thinking going on
	CancelDisplay();																	//	cancel any display going on
	theTurnStack.Reset();																//	empty the turn stack
	CMGameTurn newTurn;																	//	create an empty turn
	theTurn = newTurn;																	//	and copy it into our current turn
	theTurn.MakeToroidal(isToroidal);													//	make sure the toroidal flag is set
	showingExplosion = false;															//	set this flag, too
	theDisplayQueue.EnQueue(theTurn.startBoard);										//	put the current board back for display
	sendDisplayEvents = true;															//	and force refresh
	} // end of ResetGame()	
	
void CMassApplication::TakeBackMove()													//	reset to last human player (max 2 moves)
	{
	if ((waitingForAckRejectClicks) || (waitingForAckStopThinking))						//	if we are already cancelling
		{
		PostMessage(TakeBackItem);														//	repost the message
		snooze(10000);																	//	snooze for a while
		return;
		} // end of repetitive choice
	CancelThinking();																	//	cancel any thinking going on
	CancelDisplay();																	//	cancel any display going on
	theTurn = theTurnStack.Pop();														//	take back a move
	long playerType = (theTurn.thePlayer == RED_PLAYER) ? redPlayerType : bluePlayerType;
	if (playerType != HUMAN_PLAYER)														//	if last player was not human
		theTurn = theTurnStack.Pop();													//	take back another
	theTurn.endBoard = theTurn.startBoard;												//	and reset the ending board
	showingExplosion = false;															//	set this flag, too
	theDisplayQueue.EnQueue(theTurn.startBoard);										//	put the current board back for display
	sendDisplayEvents = true;															//	and force refresh
	} // end of ResetGame()
	
void CMassApplication::CancelThinking()													//	cancel any thinking
	{
	waitingForComputerMove = false;														//	we're no longer interested
	waitingForAckRejectClicks = true;													//	set flag saying we're waiting
	waitingForAckStopThinking = true;													//	in fact, set both
	if (pendingMove != NULL) delete pendingMove;										//	get rid of any pending move
	theCMBrain.PostMessage(CM_MSG_STOP_THINKING);										//	cancel any pending thought
	theCMWindow->PostMessage(CM_MSG_REJECT_CLICKS);										//	switch off clicks
	theCMWindow->PostMessage(CM_RESET_STATUS_BAR);										//	switch off clicks
	} // end of CancelThinking()

void CMassApplication::CancelDisplay()													//	cancel any display
	{
	sendDisplayEvents = false;															//	switch off display events
	theCMWindow->PostMessage(CM_RESET_STATUS_BAR);										//	clear the status bar
	while (doingPulse) snooze(10000);													//	wait for Pulse() to finish
	theDisplayQueue.Reset();															//	reset the display queue
	} // end of CancelDisplay()	
	
void CMassApplication::ResetPlayer(int player, int playerType)							//	reset a particular player
	{
	if (player == theTurn.thePlayer)													//	if the player is the current one
		CancelThinking();																//	cancel any move generation
	if (player == RED_PLAYER) redPlayerType = playerType;								//	reset the playertype
	else bluePlayerType = playerType;													//	for whichever
	} // end of ResetPlayer()
