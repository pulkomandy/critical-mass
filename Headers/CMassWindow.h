//	CMassWindow.h
#pragma once

#include "CMassView.h"
#include "CMassMenuBar.h"

#include <StatusBar.h>

class CMassWindow : public BWindow 								//	our window class
	{
	private:
	CMassView *theView;											//	the main view
	CMassMenuBar *theMenuBar;									//	the menu bar
	BStatusBar *theStatusBar;									//	the status bar with which to report progress

	public:
	CMassWindow(BRect frame, BBitmap **theBitmaps);				// 	constructor
	virtual	bool QuitRequested();								//	time to go
	virtual void MessageReceived(BMessage *theEvent);			//	responds to event messages
	}; // end of class CMassWindow

