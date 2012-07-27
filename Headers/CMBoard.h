//	Critical Mass Be
//	started 12/7/1997
//	Hamish Carr

#ifndef __CMBOARD_H__
#define __CMBOARD_H__

#define DEFAULT_N_ROWS 5
#define DEFAULT_N_COLS 6

#define RED_PLAYER -1
#define NEITHER_PLAYER 0
#define BLUE_PLAYER 1
#define RED_BOOM -6
#define BLUE_BOOM 6

#define PLAYER_WIPED_OUT -10000
#define ILLEGAL_MOVE -10001

#define DEFAULT_CORNER_WEIGHT 4
#define DEFAULT_EDGE_WEIGHT 2
#define DEFAULT_CENTRE_WEIGHT 1

class CMBoard												//	class encapsulating game board
{
	public:
	int **bombs;											//	the array holding the bombs
														//	note:  +ve is blue, -ve is black
	int *explosionStack;									//	stack for tracking explosions
	int stackSize;											//	size of explosion stack
	int cornerWeight, edgeWeight, centreWeight;					//	weights for various positions
	bool torusWrap;										//	whether to wrap board (ala torus)

	private:
		int nRows, nCols;

	public:
	CMBoard(int NRows = DEFAULT_N_ROWS, int NCols = DEFAULT_N_COLS,
		int CornerWeight = DEFAULT_CORNER_WEIGHT,
		int EdgeWeight = DEFAULT_EDGE_WEIGHT,
		int CentreWeight = DEFAULT_CENTRE_WEIGHT);
	CMBoard(const CMBoard &theBoard);
	~CMBoard();

	CMBoard &operator =(const CMBoard &theBoard);

	void Reset();

	inline int getWidth() { return nCols; }
	inline int getHeight() { return nRows; }

	bool IsLegalMove(int r, int c, int player);					//	test to see if a move is legal
	int Weight(int r, int c);								//	routine to find the weight of a cell
	int Evaluate();										//	evaluate the board's value 
	bool HasPlayerWon(int player);							//	routine to test for victory

	void AddBomb(int player, int r, int c);						//	routine to add a bomb
	void Move(int player, int r, int c);						//	execute a move

	bool ReadyToExplode(int r, int c);							//	routine to test whether cell will explode
 	bool StackExplosions();									//	puts squares due to explode on the stack - returns true if any exist
	void Explode(int player);								//	routine to blow up bombs - returns true if any blew up
	void ExplodeAll(int player);								//	iterates the explosion routine
	
	void Print();											//	prints board to std out
	void MakeToroidal(bool isToroidal = true);					//	switch to toroidal version
};

#endif
