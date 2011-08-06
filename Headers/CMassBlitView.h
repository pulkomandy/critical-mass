//	Critical Mass be
//	13/7/1997
//	Hamish Carr
//
//	CMassBlitView.h

#pragma once

#include "CMBoard.h"
#include "CMGUIDefs.h"

#include <View.h>

class CMassBlitView : public BView 													//	our viewing panel
	{
	public:	
	CMBoard theBoard;																//	the board currently being displayed
	bool isOpenGL;																	//	whether generating for OpenGL
	private:
	BBitmap **theBitmaps;															//	cache for the bitmaps
	BBitmap *theBlitMap;															//	the bit map for blitting

	public:
	CMassBlitView(BRect frame, char *name, BBitmap **newBitmaps, BBitmap *newBlitMap);			//	constructor
	virtual void Draw(BRect updateRect);												//	draw the view
	void Generate();																//	regenerates the contents of the view
	}; // end of class CMassBlitView
