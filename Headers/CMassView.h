//	Critical Mass be
//	13/7/1997
//	Hamish Carr
//
//	CMassView.h

#ifndef __CMASSVIEW_H__
#define __CMASSVIEW_H__

#include "CMassBlitView.h"
#include <GLView.h>
#include "Ball.h"
#include <algorithm>

const int dragBall = 0, dragObject = 1, dragScale = 2;								//	constants for which form of drag is being performed			

class CMassView : public BGLView													//	our viewing panel
{
	private:	
	CMBoard theBoard;																//	the board currently being displayed
	struct picture* theBitmaps;															//	cache for the bitmaps
	BBitmap *theBlitMap;															//	bitmap from which to blit
	CMassBlitView *theBlitView;														//	view to use to set up blitting
	bool acceptClicks;																//	whether to accept clicks from the player
	bool inGLMode;																	//	whether we are in OpenGL mode

	int torusDisplayListID;															//	integer for the display list rep. the torus
	int torusPickListID;															//	integer for the display list for picking
	float xRot, yRot, zRot;															//	direction vector for rotation							
	float scale;
	BallData ball;																	//	the arcball											
	float offset_x, offset_y;														//	allows us to move the object around laterally				
	int dragMode;																	//	start off in dragBall mode								
 	float dragOld_x, dragOld_y;														//	previous x, y coordinates of drag							
	GLuint texName;																	//	name of texture
	
	void CreateTorus(float ringRadius, float tubeRadius, GLenum mode = GL_RENDER);	//	creates display list for torus
	public:
	CMassView(BRect frame, const char *name, struct picture*newBitmaps);
	virtual void AttachedToWindow();												//	called when added to window
	virtual	void Draw(BRect updateRect);											//	draws the board
	virtual void MouseDown(BPoint where);											//	reacts to mouse clicks
	virtual void MessageReceived(BMessage *theMessage);								//	reacts to messages
	virtual void FrameResized(float neww, float newh);
	
	float marginx() {float w = Bounds().Width(); return w - ((int)w/CELL_SIZE)*CELL_SIZE;}
	float marginy() {float h = Bounds().Height(); return h - ((int)h/CELL_SIZE)*CELL_SIZE;}
	
	static int CELL_SIZE;
};

#endif
