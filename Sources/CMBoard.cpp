//	Critical Mass Be
//	started 12/7/1997
//	Hamish Carr

#include "CMBoard.h"

#include <stdlib.h>
#include <stdio.h>

CMBoard::CMBoard(int NRows, int NCols, int CornerWeight, int EdgeWeight, int CentreWeight)
{																//	default constructor
	int i, j;															//	loop indices
	nRows = NRows; nCols = NCols;											//	set the # of rows & cols
	cornerWeight = CornerWeight; edgeWeight = EdgeWeight; centreWeight = CentreWeight;
																	//	set the weighting factors
	bombs = (int **) malloc(sizeof(int *)*nRows);							//	create the array of bombs
	for (i = 0; i < nRows; i++)											//	loop through rows
		{
		bombs[i] = new int[nCols];										//	create the row
		for (j = 0; j < nCols; j++)										//	and cells
			bombs[i][j] = 0;											//	initializing to 0
		} // end of loop creating rows
	explosionStack = new int[2*nRows*nCols];								//	prepare a stack for storing explosions as r, c pairs
	stackSize = 0;														//	and set the stackSize
	torusWrap = false;													//	set initial torusWrap value
}

CMBoard::CMBoard(const CMBoard &theBoard)										//	copy constructor
{
	int i, j;															//	loop indices
	nRows = theBoard.nRows; nCols = theBoard.nCols;							//	set the # of rows & cols
	cornerWeight = theBoard.cornerWeight; edgeWeight = theBoard.edgeWeight; 
	centreWeight = theBoard.centreWeight;									//	set the weighting factors
	torusWrap = theBoard.torusWrap;										//	copy the toroidality
	bombs = (int **) malloc(sizeof(int *)*nRows);							//	create the array of bombs
	for (i = 0; i < nRows; i++)											//	loop through rows
	{
		bombs[i] = new int[nCols];										//	create the row
		for (j = 0; j < nCols; j++)										//	and cells
			bombs[i][j] = theBoard.bombs[i][j];							//	copying from the old board
	}
	explosionStack = new int[nRows*nCols];									//	prepare a stack for storing explosions
	stackSize = 0;														//	and set the stackSize
}
		
CMBoard::~CMBoard()														//	destructor
{
	for (int i = 0; i < nRows; i++) delete[] bombs[i];							//	get rid of the rows
	free(bombs);														//	get rid of the array of bombs
	delete[] explosionStack;												//	get rid of the explosion stack
} // end of destructor


CMBoard &CMBoard::operator =(const CMBoard &theBoard)
{
	int i, j;
	if ((nRows != theBoard.nRows) || (nCols != theBoard.nCols))
	{
		if (bombs != NULL)
		{
			for (i = 0; i < nRows; i++) delete bombs[i];
			free(bombs);
		}
		bombs = (int **) malloc(sizeof(int *)*nRows);
		for (i = 0; i < nRows; i++) bombs[i] = new int[nCols];
		if (explosionStack != NULL) delete explosionStack;
		explosionStack = new int[nRows*nCols];

		nRows = theBoard.nRows;
		nCols = theBoard.nCols;
	}
	torusWrap = theBoard.torusWrap;
	cornerWeight = theBoard.cornerWeight; edgeWeight = theBoard.edgeWeight; 
	centreWeight = theBoard.centreWeight;
	for (i = 0; i < nRows; i++)
		for (j = 0; j < nCols; j++)
			bombs[i][j] = theBoard.bombs[i][j];
	stackSize = 0;
	return *this;
}

	
bool CMBoard::IsLegalMove(int r, int c, int player)							//	test to see if a move is legal
	{
	return ((bombs[r][c]*player) >= 0);									//	calculate legality & return it
	} // end of IsLegalMove()

int CMBoard::Weight(int r, int c)											//	routine to compute the weight of a cell
	{
	if ((r == 0) || (r == nRows - 1))										//	i.e. in an edge row
		if ((c == 0) || (c == nCols - 1))									//	and an edge column
			return cornerWeight;										//	return the corner weight
		else 
			return edgeWeight;											//	return the edge weight
	else 
		if ((c == 0) || (c == nCols - 1))									//	and an edge column
			return edgeWeight;											//	return the edge weight
		else
			return centreWeight;										//	explodes if at least four bombs
	} // end of Weight()

int CMBoard::Evaluate()													//	evaluate the board's value 
	{
	int i, j;															//	loop indices
	int value = 0;														//	the total value of the board
	int nBombsTotal = 0;												//	total # of bombs on the board
	bool hasRed = false, hasBlue = false;									//	flags for whether a player has been eliminated

	for (i = 0; i < nRows; i++)											//	loop through rows
		for (j = 0; j < nCols; j++)										//	and cells
			{
			hasRed |= (bombs[i][j] < 0);									//	update the hasRed flag
			hasBlue |= (bombs[i][j] > 0);									//	and the hasBlue flag
			value += Weight(i, j)*bombs[i][j];								//	update the value
			nBombsTotal += abs(bombs[i][j]);								//	add to the total bombs
			} // end of loop through cells
	if ((!hasRed) && (nBombsTotal > 2))									//	red is first player, so we check that first
		return RED_PLAYER * PLAYER_WIPED_OUT;								//	if there aren't any, red has lost
	if ((!hasBlue) && (nBombsTotal > 2))									//	check blue next
		return BLUE_PLAYER * PLAYER_WIPED_OUT;								//	if there aren't any, blue has lost
	return value;														//	otherwise, return calculated value
	} // end of Evaluate()

bool CMBoard::HasPlayerWon(int player)										//	routine to test for victory
	{
	return ((Evaluate() * player) == -PLAYER_WIPED_OUT);						//	call Evaluate & return
	} // end of HasPlayerWon()

void CMBoard::AddBomb(int player, int r, int c)								//	routine to add a bomb
	{
	bombs[r][c] += player;												//	add/subtract bomb in cell
	} // end of AddBomb

void CMBoard::Move(int player, int r, int c)									//	execute a move
	{
	AddBomb(player, r, c);												//	add the bomb
	ExplodeAll(player);													//	and do the explosions
	} // end of Move()

bool CMBoard::ReadyToExplode(int r, int c)									//	routine to test whether cell will explode
	{
	if (torusWrap) return (abs(bombs[r][c]) >= 4);							//	tori need 4 in a cell
	if ((r == 0) || (r == nRows - 1))										//	i.e. in an edge row
		if ((c == 0) || (c == nCols - 1))									//	and an edge column
			return (abs(bombs[r][c]) >= 2);								//	explodes if at least two bombs
		else
			return (abs(bombs[r][c]) >= 3);								//	explodes if at least three bombs
	else
		if ((c == 0) || (c == nCols - 1))									//	and an edge column
			return (abs(bombs[r][c]) >= 3);								//	explodes if at least three bombs
		else
			return (abs(bombs[r][c]) >= 4);								//	explodes if at least four bombs
	} // end of ReadyToExplode()

bool CMBoard::StackExplosions()											//	puts squares due to explode on the stack - returns true if any exist
	{
	int i, j;															//	loop indices
	stackSize = 0;														//	reset the stack of explosions
	for (i = 0; i < nRows; i++)											//	loop through rows
		for (j = 0; j < nCols; j++)										//	and cells
			if (ReadyToExplode(i, j))									//	if the cell is ready to explode
				{
				if (bombs[i][j] < 0) bombs[i][j] = RED_BOOM;					//	mark it with an explosion
				else bombs[i][j] = BLUE_BOOM;								//	or with a blue explosion
				explosionStack[stackSize++] = i;							//	add the i coordinate
				explosionStack[stackSize++] = j;							//	and j
				} // end of case where explosion found
	return (stackSize > 0);												//	if there are no explosions to do
	} // end of StackExplosions()

void CMBoard::Explode(int player)											//	routine to blow up bombs - returns true if any blew up
	{
	int i, j;															//	loop indices
	for (; stackSize > 0; )												//	walk back down the stack
		{
		j = explosionStack[--stackSize];									// 	retrieve the coordinates
		i = explosionStack[--stackSize];									// 	in reverse order
		bombs[i][j] = 0;												//	set the cell to empty
		if (i > 0) bombs[i-1][j] = player*(1 + abs(bombs[i-1][j]));				//	throw a bomb upwards
		else if (torusWrap) bombs[nRows-1][j] = player*(1+abs(bombs[nRows-1][j]));	//	do the same on a torus only
		if (i < nRows - 1) bombs[i+1][j] = player*(1 + abs(bombs[i+1][j]));		//	throw a bomb downwards
		else if (torusWrap) bombs[0][j] = player*(1+abs(bombs[0][j]));			//	do the same on a torus only
		if (j > 0) bombs[i][j-1] = player*(1 + abs(bombs[i][j-1]));				//	throw a bomb to the left
		else if (torusWrap) bombs[i][nCols-1] = player*(1+abs(bombs[i][nCols-1]));	//	do the same on a torus only
		if (j < nCols - 1) bombs[i][j+1] = player*(1 + abs(bombs[i][j+1]));		//	throw a bomb to the right
		else if (torusWrap) bombs[i][0] = player*(1+abs(bombs[i][0]));			//	do the same on a torus only
		} // end of loop for explosions
	} // end of Explode()

void CMBoard::ExplodeAll(int player)										//	iterates the explosion routine
	{
	bool explosionsExist;												//	flag for whether explosions happen
	for (;;)															//	loop forever
		{
		if (HasPlayerWon(player)) return;									//	if the player has won, leave now
		explosionsExist = StackExplosions();								//	call routine to create stack
		if (!explosionsExist)											//	if there are no explosions to do
			return;													//	leave the main loop
		Explode(player);												//	do a round of explosions
		} // end of outer for loop
	} // end of Explode()	

void CMBoard::Print()													//	prints board to std out
	{
	int i, j;															//	loop indices
	for (i = 0; i < nRows; i++)											//	loop through rows
		{
		for (j = 0; j < nCols; j++)										//	and cells
			{
			if (bombs[i][j] < 0)										//	i.e. a red square
				printf("%6dR ", -bombs[i][j]);							//	print out red
			else if (bombs[i][j] > 0)									//	i.e. a blue square
				printf("%6dB ", bombs[i][j]);								//	print out blue
			else printf("%6d  ", 0);										//	otherwise, print 0
			} // end of loop along rows
		printf("\n");													//	terminate the row
		} // end of loop down rows
	} // end of Print()

void CMBoard::Reset()													//	resets the bombs to empty
	{
	int i, j;															//	loop indices
	for (i = 0; i < nRows; i++)											//	loop through rows
		for (j = 0; j < nCols; j++)										//	and cells
			bombs[i][j] = 0;											//	set cell to empty
	} // end of reset
	 
void CMBoard::MakeToroidal(bool isToroidal)									//	switch to toroidal version
	{
	torusWrap = isToroidal;												//	switch to toroidal form
	cornerWeight = torusWrap ? 1 : DEFAULT_CORNER_WEIGHT;						//	set weights
	edgeWeight = torusWrap ? 1 : DEFAULT_EDGE_WEIGHT;							//	set weights
	centreWeight = torusWrap ? 1 : DEFAULT_CENTRE_WEIGHT;						//	set weights
	} // end of MakeToroidal()	 
	 
