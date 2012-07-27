//	CMassWindow.h

#ifndef __CMASSWINDOW_H__
#define __CMASSWINDOW_H__

#include "CMassView.h"
#include "CMassMenuBar.h"
#include "CMassApplication.h"

#include <StatusBar.h>

class CMassWindow : public BWindow 								//	our window class
	{
	private:
	CMassView *theView;											//	the main view
	CMassMenuBar *theMenuBar;									//	the menu bar
	BStatusBar *theStatusBar;									//	the status bar with which to report progress

	public:
	CMassWindow(BRect frame, struct picture* theBitmaps);
	virtual	bool QuitRequested();								//	time to go
	virtual void MessageReceived(BMessage *theEvent);			//	responds to event messages
	}; // end of class CMassWindow

#endif
