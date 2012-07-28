//	Critical Mass
//	13/7/1997
//	Hamish Carr
//	CMassView.cpp

#include "CMassView.h"
#include "Ball.h"

#include <algorithm>

#include <GLView.h>

CMassView::CMassView(BRect frame, const char *name, struct picture* newBitmaps)					//	constructor
	   	   : BGLView(frame, name, B_FOLLOW_ALL, B_FULL_UPDATE_ON_RESIZE, BGL_RGB | BGL_DEPTH | BGL_DOUBLE)//	call inherited constructor
	{
	CELL_SIZE = 63;
	theBitmaps = newBitmaps;														//	save a pointer to the bitmaps
	acceptClicks = false;															//	and ignore any clicks
	inGLMode = false;																//	start off NOT in GL mode

	xRot = 0.6; yRot = 0.8; zRot = 0.0;												//	direction vector for rotation							
	scale = 25.0;
	offset_x = 0.0; offset_y = 0.0;													//	allows us to move the object around laterally				
	dragMode = dragBall;															//	start off in dragBall mode								
	Ball_Init(&ball);																//	initialize the arcball									
	Ball_Place(&ball, qOne, 0.75);													//	place the arcball (at 3/4 of window size)
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	} // end of CMassView constructor

void CMassView::AttachedToWindow()													//	called when added to window
{
	GLfloat light_position[] = {0.0, 3.0, 4.0, 0.0};								// position of light source (at infinity)			
	GLfloat light_colour[] = {1.0, 1.0, 1.0, 0.0};									//	colour of light source

	GLfloat torus_colour[] = {1.0, 1.0, 1.0, 0.0};									// set the torus to green								
//	GLfloat torus_specular_colour[] = {1.0, 1.0, 1.0, 0.0};							// and set shininess up a little bit
	BGLView::AttachedToWindow();													//	call inherited attachment function
	LockGL();																		//	make sure this is thread-safe

	glMatrixMode(GL_PROJECTION);													//	first set up matrix for the viewing volume	
	glOrtho(-240.0, 240.0, -200.0, 200.0, -1000.0, 1000.0);							//	set the perspective									
 	glMatrixMode(GL_MODELVIEW);														// switch to matrix for basis vectors at eye		
	glLoadIdentity();																//	load the identity matrix
	glTranslatef(0, 0, -600.0);														//	translate the model matrix									
//	glRotatef(30.0, 0.0, 1.0, 0.0);													//	rotate the torus
	torusPickListID = glGenLists(2);												//	set up a total of 1 display list					
	glNewList(torusPickListID, GL_COMPILE);											//	tell it to save the following calls			
	CreateTorus(100.0, 50.0, GL_SELECT);											//	set up a list for rendering
	glEndList();																	// close the list											
	glShadeModel(GL_SMOOTH);														// use smooth lighting										
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, torus_colour);					// set the colour of the torus						
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);								// specify the position of the light				
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_colour);									//	and the colour
	glEnable(GL_LIGHTING);															// switch lighting on									
	glEnable(GL_LIGHT0);															// switch light #0 on									
	glEnable(GL_DEPTH_TEST);														//	make sure depth buffer is switched on			
	glEnable(GL_TEXTURE_2D);														//	enable 2D textures
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);											//	set the pixel alignment
	glGenTextures(1, &texName);														//	create the GL texture "name"
	glBindTexture(GL_TEXTURE_2D, texName);											//	mark it as 2D
	
	torusDisplayListID = torusPickListID + 1;										//	we know this name is available
	glNewList(torusDisplayListID, GL_COMPILE);										//	tell it to save the following calls				
	CreateTorus(100.0, 50.0, GL_RENDER);											//	set up a list for rendering
	glEndList();																	// close the list											
	UnlockGL();																		//	unlock GL

	FrameResized(Bounds().Width(), Bounds().Height());
	
} // end of AttachedToWindow()
	
void CMassView::Draw(BRect updateRect)												//	draw the view
	{
	if (!inGLMode)
		{
		//DrawBitmap(theBlitMap, updateRect, updateRect);								//	blit the bits
		return;
		} // end of simple case
	else
		{
		LockGL();																	//	lock in preparation for drawing
		GLfloat mNow[16];															//	local matrix for ball routines							
		Ball_Update(&ball);															//	update the data for the ball								
		Ball_Value(&ball, mNow);													//	retrieve the ball's position as a matrix						
		glMatrixMode(GL_MODELVIEW);													//	make sure that we are set to the model matrix				
		glClearColor(0.0, 0.0, 0.0, 1.0);											//	and set the "clear" colour to black						
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);							//	clear the window to black								
		glPushMatrix();																//	push the GL stack to get a new matrix						
		glLoadIdentity();															//	load the identity matrix								
		glTranslatef(0, 0, -600.0);													//	translate the model matrix									
		glMultMatrixf(mNow);														//	multiply by this matrix								
		glEnable(GL_TEXTURE_2D);													//	enable 2D textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);				//	set the vertical (?) wrap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);				//	and the horizontal
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			//	set magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			//	set shrinking filter
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, theBlitMap->Bounds().Width() + 1, theBlitMap->Bounds().Height() + 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, theBlitMap->Bits());
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);				//	just stick the texture on		
		glBindTexture(GL_TEXTURE_2D, texName);										//	bind the texture to the name (again?)
		glCallList(torusDisplayListID);												// and call the display list							
		glPopMatrix();																//	pop the matrix back off the stack
		SwapBuffers();																//	swap the buffers to draw it
		glDisable(GL_TEXTURE_2D);													//	switch textures back off
		UnlockGL();																	//	unlock GL
		} // end of GL case	
	} // end of Draw()

void CMassView::MouseDown(BPoint where)												//	reacts to mouse clicks
	{
	long whichButtons = 1;															//	used for tracking which buttons are down
	Window()->CurrentMessage()->FindInt32("buttons", &whichButtons);				//	find out which buttons are down
	
	if (inGLMode && (whichButtons & B_SECONDARY_MOUSE_BUTTON))						//	if we are in GL mode, and button 2 is down
		{
		float frameWidth = Frame().Width(), frameHeight = Frame().Height();			//	find the width & height
		dragMode = dragBall;														//	drag the arcball to rotate									
		HVect vNow;																	//	vector type for passing to ball functions					
		vNow.x = (2.0 * where.x - frameWidth)/frameWidth;							//	set the vector										
		vNow.y = -(2.0 * where.y - frameHeight)/frameHeight;							//	in both dimensions									
		Ball_Mouse(&ball, vNow);													//	and pass it to the Ball functions							
		Ball_BeginDrag(&ball);														//	start dragging										
		while (whichButtons)														//	loop until drop the mouse
			{
			snooze(20 * 1000);														//	snooze for 20 µs
			GetMouse(&where, (ulong *)&whichButtons, true);									//	get the mouse location, &c.
			vNow.x = (2.0 * where.x - frameWidth)/frameWidth;						//	set the vector	
			vNow.y = -(2.0 * where.y - frameHeight)/frameHeight;						//	in both dimensions
			Ball_Mouse(&ball, vNow);												//	and pass it to the Ball functions
			Draw(Frame());															//	redraw the entire frame
			} // end of while (whichButtons)
		Ball_EndDrag(&ball);														//	stop dragging	
		} // end of case where dragging

	else if (acceptClicks)															//	if we have "accept" switched on
		{
		long row, col;																//	the board coordinates of the click
		if (!inGLMode)																//	if it's the regular board
			{
			row = where.y / CELL_SIZE;												//	calculate which row to look in
			col = where.x / CELL_SIZE;												//	and which column
			} // end of normal mode
		else 
			{
			GLubyte theColour[4];													//	array for retrieving "colour"
			LockGL();																//	lock in preparation for drawing
			GLfloat mNow[16];														//	local matrix for ball routines							
			Ball_Update(&ball);														//	update the data for the ball								
			Ball_Value(&ball, mNow);												//	retrieve the ball's position as a matrix						
			glDisable(GL_LIGHTING);													//	disable lighting
			glShadeModel(GL_FLAT);													//	switch to flat shading
			glMatrixMode(GL_MODELVIEW);												//	make sure that we are set to the model matrix				
			glClearColor(0.0, 0.0, 0.0, 1.0);										//	and set the "clear" colour to black						
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);						//	clear the window to black								
			glPushMatrix();															//	push the GL stack to get a new matrix						
			glLoadIdentity();														//	load the identity matrix								
			glTranslatef(0, 0, -600.0);												//	translate the model matrix									
			glMultMatrixf(mNow);													//	multiply by this matrix								
			glCallList(torusPickListID);											// and call the display list							
			glReadPixels(where.x, Frame().Height() - where.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, theColour);
																					//	read a single pixel at the mouse location
			glPopMatrix();															//	pop the matrix back off the stack
			glEnable(GL_LIGHTING);													//	re-enable lighting
			glShadeModel(GL_SMOOTH);												//	and smoothness
			UnlockGL();																//	unlock GL
			Draw(Frame());															//	redraw the entire picture
			row = theColour[1] - 128; col = theColour[0] - 128;						//	retrieve the row & column
																					//	(background is black & returns -128)
//			printf("%d %d\n", row, col); return;
			} // end of GL mode code
		if (row < 0) return;														//	make sure it is a legal cell
		else if (row >= theBoard.getHeight()) return;										//	i.e. not off top or bottom
		if (col < 0) return;														//	same with left & right
		else if (col >= theBoard.getWidth()) return;	
		
		BMessage *theMessage = new BMessage(CM_MSG_MOVE_CHOSEN);					//	create a message for it
		acceptClicks = false;														//	turn off "accept clicks"
		theMessage->AddInt32("row", row); theMessage->AddInt32("column", col);		//	add the coordinates to the message
		be_app->PostMessage(theMessage);											//	send the message off
		delete theMessage;															//	get rid of the message when done
		} // end of case where we accept clicks
	} // end of MouseDown()

void CMassView::MessageReceived(BMessage *theMessage)								//	reacts to messages
	{
	switch (theMessage->what)														//	look at the message's type 
		{
		case CM_MSG_ACCEPT_CLICKS:													//	the game is waiting for the user's move
			acceptClicks = true;													//	switch on click acceptance
			break;
		case CM_MSG_REJECT_CLICKS:													//	the game wants us to stop accepting click
		{
			acceptClicks = false;													//	set it so we no longer accept clicks
			BMessage *response = new BMessage(CM_MSG_ACK_REJECT_CLICKS);			//	set up a responding message
			be_app->PostMessage(response);											//	and send it back
			break;
		}
		case CM_MSG_SHOW_BOARD:
		{
			CMBoard *sentBoard = NULL;
			status_t errCode = theMessage->FindPointer("displayBoard", (void**)&sentBoard);
			if (errCode == B_NO_ERROR)
			{
				theBlitView->theBoard = theBoard = *sentBoard;
				theBlitView->Generate();
				Invalidate();
			} // end of successful board transfer
			break;
		}
		case OptionRenderTorusItem:													//	toggle OpenGL mode
			inGLMode = !inGLMode;													//	just do it
			theBlitView->isOpenGL = inGLMode;										//	make sure that theBlitView is the same
			theBlitView->Generate();												//	force the blitview to regenerate
			Invalidate();															//	invalidate the whole view
			break;
		default:																	//	if we don't recognize it
			BGLView::MessageReceived(theMessage);									//	let the inherited method deal with it
		} // end of switch
	} // end of MessageReceived
	
void CMassView::CreateTorus(float ringRadius, float tubeRadius, GLenum mode)
{
	int i, j;
	int ringDivisions = theBoard.getHeight()*8;
	int tubeDivisions = theBoard.getWidth()*8;
	GLfloat theta, phi, theta1, phi1, thetaNorm, phiNorm, theta1Norm, phi1Norm;
	GLfloat p0[03], p1[3], p2[3], p3[3];
	GLfloat n0[3], n1[3], n2[3], n3[3];

	for (i = 0; i < ringDivisions; i++) 
		{
		thetaNorm = (GLfloat) i / ringDivisions;
		theta = (GLfloat) thetaNorm *2.0 * M_PI;
		theta1Norm = (GLfloat) (i + 1) / ringDivisions;
		theta1 = (GLfloat) theta1Norm *2.0 * M_PI;
		for (j = 0; j < tubeDivisions; j++) 
			{
			phiNorm = (GLfloat) j / tubeDivisions;
			phi = phiNorm * 2.0 * M_PI;
			phi1Norm = (GLfloat) (j + 1) / tubeDivisions;
			phi1 = phi1Norm * 2.0 * M_PI;

			p0[0] = cos(theta) * (ringRadius + tubeRadius * cos(phi));
			p0[1] = -sin(theta) * (ringRadius + tubeRadius * cos(phi));
			p0[2] = tubeRadius * sin(phi);

			p1[0] = cos(theta1) * (ringRadius + tubeRadius * cos(phi));
			p1[1] = -sin(theta1) * (ringRadius + tubeRadius * cos(phi));
			p1[2] = tubeRadius * sin(phi);

			p2[0] = cos(theta1) * (ringRadius + tubeRadius * cos(phi1));
			p2[1] = -sin(theta1) * (ringRadius + tubeRadius * cos(phi1));
			p2[2] = tubeRadius * sin(phi1);

			p3[0] = cos(theta) * (ringRadius + tubeRadius * cos(phi1));
			p3[1] = -sin(theta) * (ringRadius + tubeRadius * cos(phi1));
			p3[2] = tubeRadius * sin(phi1);

			n0[0] = cos(theta) * (cos(phi));
			n0[1] = -sin(theta) * (cos(phi));
			n0[2] = sin(phi);

			n1[0] = cos(theta1) * (cos(phi));
			n1[1] = -sin(theta1) * (cos(phi));
			n1[2] = sin(phi);

			n2[0] = cos(theta1) * (cos(phi1));
			n2[1] = -sin(theta1) * (cos(phi1));
			n2[2] = sin(phi1);

			n3[0] = cos(theta) * (cos(phi1));
			n3[1] = -sin(theta) * (cos(phi1));
			n3[2] = sin(phi1);

			glBegin(GL_QUADS);
			if (mode == GL_SELECT)
				glColor3ub(128+i/8, 128+j/8, 0);													// set the colour of the torus						
			glTexCoord2f((float)i/64, (float)(j+1)/64);
			glNormal3fv(n3);
			glVertex3fv(p3);
			glTexCoord2f((float)(i+1)/64, (float)(j+1)/64);
			glNormal3fv(n2);
			glVertex3fv(p2);
			glTexCoord2f((float)(i+1)/64, (float)j/64);
			glNormal3fv(n1);
			glVertex3fv(p1);
			glTexCoord2f((float)i/64, (float)j/64);
			glNormal3fv(n0);
			glVertex3fv(p0);
			glEnd();
			} // end of loop around tube
		} //end of loop around ring
	} // end of CreateTorus


void CMassView::FrameResized(float neww, float newh)
{
	if (neww < 32 || newh < 32) return;
	CELL_SIZE = std::min(Bounds().Width()/DEFAULT_N_COLS, Bounds().Height()/DEFAULT_N_ROWS);
	//BRect bitampSize(0, 0, CELL_SIZE * DEFAULT_N_COLS, CELL_SIZE * DEFAULT_N_ROWS);
	BRect bitmapSize = Bounds();
	theBlitMap = new BBitmap(bitmapSize, B_RGB_32_BIT, true);							//	create blitmap
	theBlitView = new CMassBlitView(bitmapSize, "BlitView", theBitmaps, theBlitMap);	//	create the blitview
	theBlitView->theBoard = theBoard;
	// TODO ? set this to true in OpenGL mode ? (and use a 512x512 rect ?)
	theBlitView->isOpenGL = false;													//	and start in OpenGL mode
	theBlitMap->AddChild(theBlitView);												//	add the blitview to the blitmap
	theBlitView->Generate();														//	fill in the blitmap
	SetViewBitmap(theBlitMap, B_FOLLOW_TOP | B_FOLLOW_LEFT, false);
}
	
int CMassView::CELL_SIZE;
