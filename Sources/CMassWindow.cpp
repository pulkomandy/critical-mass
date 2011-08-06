//	Critical Mass Be
//	13/7/97
//	Hamish Carr
//
//	CMassWindow.cpp

#include "CMassWindow.h"

CMassWindow::CMassWindow(BRect frame, BBitmap **theBitmaps)							//	constructor
				: BWindow(frame, "Critical Mass", B_TITLED_WINDOW, B_NOT_RESIZABLE)	//	calls inherited constructor
	{																		
	BRect viewRect;																	//	used for setting sizes
	viewRect.Set(0, 20, 383, 340);													//	set to a suitable size for the board
	theView = new CMassView(viewRect, "Critical Mass View", theBitmaps);			//	initialize the board view
	AddChild(theView);																//	add the view as a child to this window
	viewRect.Set(0, 0, 383, 20);													//	set the size of the menu bar
	theMenuBar = new CMassMenuBar(viewRect, "Critical Mass Menu Bar");				//	initialize the menu bar
	AddChild(theMenuBar);															//	add the menu to the window
	theMenuBar->AttachedToWindow();													//	and tell it that it is attached
	viewRect.Set(0, 340, 383, 370);													//	set the rect size for status bar
	theStatusBar = new BStatusBar(viewRect, "GedankenBar", NULL, NULL);				//	create the status bar
	theStatusBar->SetMaxValue(100.0);												//	set the maximum size of the bar
	AddChild(theStatusBar);															//	connect up the status bar
	} // end of CMassWindow constructor

bool CMassWindow::QuitRequested()													//	called when app wants to quit
	{
	be_app->PostMessage(B_QUIT_REQUESTED);											//	ask the app to quit
	return(TRUE);																	//	and say it's OK by us
	} // end of QuitRequested()

void CMassWindow::MessageReceived(BMessage *theEvent)								//	responds to event messages
	{
	switch (theEvent->what)															//	just a big switch statement
		{
		case CM_MSG_ACCEPT_CLICKS:													//	tells view to accept clicks
		case CM_MSG_REJECT_CLICKS:													//	tells view to reject clicks
		case CM_MSG_SHOW_BOARD:														//	tells the view to show a board
			theView->MessageReceived(theEvent);										//	pass it on to the view
			break;
			
		case CM_RESET_STATUS_BAR:													//	tells the window to reset the status bar
			theStatusBar->Reset();													//	reset the status bar
			break;

		case CM_SHOW_PERCENT:														//	tells the window to update percent in status bar
		{
			float sentPercent;														//	used to retrieve the value we are sent
			status_t errCode = theEvent->FindFloat("percent", &sentPercent);		//	retrieve the value
			if (errCode == B_NO_ERROR)												//	if it was successful
				theStatusBar->Update(sentPercent - theStatusBar->CurrentValue(), NULL, NULL);
			break;
		}
		case CM_MAKE_STATUS_BAR_RED:												//	tells the window to make status bar red
		{
			rgb_color red_colour = {255, 0, 0, 0};									//	set the colour
			theStatusBar->SetBarColor(red_colour);									//	do it
			theStatusBar->SetText("Thinking . . .");								//	set the message
			break;
		}

		case CM_MAKE_STATUS_BAR_BLUE:												//	tells the window to make status bar blue
		{
			rgb_color blue_colour = {0, 0, 255, 0};									//	set the colour
			theStatusBar->SetBarColor(blue_colour);									//	do it
			theStatusBar->SetText("Thinking . . .");								//	set the message
			break;
		}

		case CM_MARK_SOUND_ON:														//	tells window to mark sound as being on
		case CM_MARK_SOUND_OFF:														//	tells window to mark sound as being off
		case Player1HumanItem:														//	define the red player to be human
		case Player1RandomItem:														//	define the red player to be random
		case Player1SmartItem:														//	define the red player to be smart
		case Player1AIItem:															//	define the red player to be AI
		case Player1IsBillKocayItem:												//	define the red player to be Bill Kocay
		case Player1IsAlbertEinsteinItem:											//	define the red player to be Albert Einstein
		case Player2HumanItem:														//	define the blue player to be human
		case Player2RandomItem:														//	define the blue player to be random
		case Player2SmartItem:														//	define the blue player to be smart
		case Player2AIItem:															//	define the blue player to be AI
		case Player2IsBillKocayItem:												//	define the blue player to be Bill Kocay
		case Player2IsAlbertEinsteinItem:											//	define the blue player to be Albert Einstein
		case OptionFastDisplayItem:													//	fast display mode
		case OptionMediumDisplayItem:												//	medium display mode
		case OptionSlowDisplayItem:													//	slow display mode
		case OptionWrapAroundItem:													//	wraparound mode
		case LowPriorityItem:														//	switch to low priority thought
		case NormalPriorityItem:													//	switch to normal priority thought
		case DisplayPriorityItem:													//	switch to display priority thought
		case UrgentDisplayPriorityItem:												//	switch to urgent display priority thought
		case RealTimeDisplayPriorityItem:											//	switch to real time display priority thought
		case UrgentPriorityItem:													//	switch to urgent priority thought
		case RealTimePriorityItem:													//	switch to real time priority thought
			theMenuBar->MessageReceived(theEvent);									//	pass all of these on to the menu bar
			break;
		case OptionRenderTorusItem:													//	toggle GL view
			theMenuBar->MessageReceived(theEvent);									//	pass all of these on to the menu bar
			theView->MessageReceived(theEvent);									//	pass to the view as well
			break;					
			
		default:																	//	if we don't recognize it
			BWindow::MessageReceived(theEvent);										//	pass it on to inherited method
		} // end of switch on theEvent->what
	} // end of MessageReceived
	
