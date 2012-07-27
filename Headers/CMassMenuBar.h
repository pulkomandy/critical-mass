//	Critical Mass be
//	31/10/1998
//	Hamish Carr
//
//	CMassMenuBar.h

#ifndef __CMMENUBAR_H__
#define __CMMENUBAR_H__

#include "CMGUIDefs.h"

#include <MenuBar.h>

class CMassMenuBar : public BMenuBar											//	our viewing panel
	{
	private:
	BMenuItem *torusItem;													//	the magic item
	public:
	CMassMenuBar(BRect frame, char *name);										//	constructor
	virtual void MessageReceived(BMessage *theMessage);							//	reacts to messages
	}; // end of class CMassMenuBar

#endif
