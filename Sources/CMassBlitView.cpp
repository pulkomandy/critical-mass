//	Critical Mass be
//	13/7/1997
//	Hamish Carr
//
//	CMassBlitView.cpp

#include "CMassBlitView.h"
#include <Bitmap.h>

CMassBlitView::CMassBlitView(BRect frame, char *name, BBitmap **newBitmaps, BBitmap *newBlitMap)	//	constructor
	: BView(frame, name, B_FOLLOW_ALL, B_WILL_DRAW)										//	call inherited constructor
	{
	theBitmaps = newBitmaps;															//	save the bitmap pointer
	theBlitMap = newBlitMap;															//	save the blitmap
	} // end of constructor
	
void CMassBlitView::Draw(BRect updateRect)												//	draw the view
	{
	#pragma unused(updateRect)														//	we ignore this
	int i,j;																		//	loop indices
	BPoint BombPoint;																//	where to draw the bomb
	long nBombs;																	//	the number of bombs in a cell
		
	for (i = 0; i < theBoard.nRows; i++)												//	loop down the rows
		{
		for (j = 0; j < theBoard.nCols; j++)											//	walk down the columns
			{
			BombPoint.Set((j)*CELL_SIZE, (i)*CELL_SIZE);									//	compute the TL corner of the cell					
			MovePenTo(BombPoint);													//	move the pen into place
			nBombs = theBoard.bombs[i][j];											//	retrieve the number of bombs
			if (isOpenGL)															//	crude hack to reverse colours
				nBombs = CELL_EMPTY - nBombs;											//	reverse the bitmaps
			else
				nBombs += CELL_EMPTY;												//	add this to get the correct index
			DrawBitmap(theBitmaps[nBombs]);											//	and draw the bitmap
			} // end of for j loop
		} // end of for i loop	
	Sync();																		//	wait for the server to draw it
	} // end of Draw()
	
void CMassBlitView::Generate()														//	regenerates the contents of the view
	{
	theBlitMap->Lock();																//	lock the blitmap first
	Draw(Bounds());																//	draw the entire bitmap
	theBlitMap->Unlock();															//	unlock the blitmap
	} // end of Generate()
