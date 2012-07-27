//	Critical Mass be
//	13/7/1997
//	Hamish Carr
//
//	CMassBlitView.cpp

#include "CMassBlitView.h"
#include <Bitmap.h>
#include <errno.h>
#include <cstdio>

#include <CMassApplication.h>
#include <IconUtils.h>
#include <InterfaceDefs.h>

CMassBlitView::CMassBlitView(BRect frame, char *name, struct picture* newBitmaps, BBitmap *newBlitMap)	//	constructor
	: BView(frame, name, B_FOLLOW_ALL, B_WILL_DRAW)										//	call inherited constructor
	{
	theBitmaps = newBitmaps;															//	save the bitmap pointer
	theBlitMap = newBlitMap;															//	save the blitmap
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	} // end of constructor
	
void CMassBlitView::Draw(BRect /*updateRect*/)												//	draw the view
	{
	BPoint BombPoint;																//	where to draw the bomb
	long nBombs;
	int CELL_SIZE = CMassView::CELL_SIZE;
	
	SetDrawingMode(B_OP_OVER);

	// Generate 1 more column and line to make sure we cover the area around the playfield...
	for (int i = theBoard.getHeight() + 1 ; --i >= 0;)
	{
		for (int j = theBoard.getWidth() + 1 ; --j >= 0;)
		{
			if ((i * theBoard.getHeight() + j)%2)
				SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
			else
				SetHighColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), B_DARKEN_1_TINT));
			BRect start (j * CELL_SIZE, i * CELL_SIZE, (j+1) * CELL_SIZE, (i+1) * CELL_SIZE);
			FillRect(start);
		}
	}
	
	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);	
		
	for (int i = 0; i < theBoard.getHeight(); i++)												//	loop down the rows
		{
		for (int j = 0; j < theBoard.getWidth(); j++)											//	walk down the columns
			{
			BombPoint.Set((j)*CELL_SIZE, (i)*CELL_SIZE);									//	compute the TL corner of the cell					
			MovePenTo(BombPoint);													//	move the pen into place
			nBombs = theBoard.bombs[i][j];											//	retrieve the number of bombs
			if (isOpenGL)															//	crude hack to reverse colours
				nBombs = CELL_EMPTY - nBombs;											//	reverse the bitmaps
			else
				nBombs += CELL_EMPTY;
			BRect size(0, 0, CELL_SIZE, CELL_SIZE);
			BBitmap rendered(size, 0, B_RGB32);
			BIconUtils::GetVectorIcon((const uint8*)theBitmaps[nBombs].bitmap, theBitmaps[nBombs].len, &rendered);
			DrawBitmap/*Async*/(&rendered);
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
