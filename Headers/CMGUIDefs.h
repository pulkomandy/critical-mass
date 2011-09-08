//	Critical Mass
//	31/X/98
//	Hamish Carr

//	GUI declarations

#define N_BITMAPS 13 // 1-5 red / blue, booms, white
#define CELL_EMPTY 6 // offset into array of bitmaps

// error messages
#define DIE_BAD_ENTRY SUICIDE_NOTE"I couldn't find the file with the pretty pictures (BEntry failed)"
#define DIE_BAD_FILE SUICIDE_NOTE"I couldn't find the file with the pretty pictures (BFile failed)"
#define DIE_BAD_RESOURCES SUICIDE_NOTE"I couldn't open the file with the pretty pictures (BResources failed)"
#define DIE_BAD_PICT SUICIDE_NOTE"I couldn't find one of the pictures (FindResource() failed)"
#define DIE_BAD_SOUND SUICIDE_NOTE"I couldn't find one of the sounds (FindResource() failed)"

// display speeds
	
#define SLOW_SPEED 1200000		//	i.e. 1.2 second
#define MEDIUM_SPEED 600000		//	i.e. 0.6 second
#define FAST_SPEED 300000		//	i.e. 0.3 second

// player types

#define HUMAN_PLAYER 0			//	types for the players
#define RANDOM_PLAYER 1			//	random choice 
#define SMART_PLAYER 2			//	board evaluator
#define AI_PLAYER 3				//	shallow lookahead
#define BILL_KOCAY_PLAYER 4		//	deep lookahead
#define ALBERT_EINSTEIN_PLAYER 5 // very deep lookahead

// messages between components
#define CM_MSG_ACCEPT_CLICKS 		2000L					//	tells view to accept clicks
#define CM_MSG_REJECT_CLICKS 		2001L					//	tells view to reject clicks
#define CM_MSG_ACK_REJECT_CLICKS	2002L					//	view acknowledges "reject" message
#define CM_MSG_SHOW_BOARD			2003L					//	tells the view to show a board
#define CM_MSG_MOVE_CHOSEN 			2004L					//	message from view to app with move
#define CM_MSG_DISPLAY_DONE			2005L					//	message that display sequence is done

#define CM_RESET_STATUS_BAR			3000L					//	tells the window to reset the status bar
#define CM_SHOW_PERCENT				3001L					//	tells the window to update percent in status bar
#define CM_MAKE_STATUS_BAR_RED		3002L					//	tells the window to make status bar red
#define CM_MAKE_STATUS_BAR_BLUE		3003L					//	tells the window to make status bar blue

#define CM_MARK_SOUND_ON			4000L					//	tells window to mark sound as being on
#define CM_MARK_SOUND_OFF			4001L					//	tells window to mark sound as being off

#define CM_MSG_STOP_THINKING		5000L					//	message to brain to stop thinking
#define CM_MSG_ACK_STOP_THINKING	5001L					//	acknowledgement
#define CM_MSG_MAKE_MOVE			5002L					//	instruction to brain to come up with move
#define CM_MSG_CANT_MAKE_MOVE		5003L					//	brain doesn't understand request

#define CM_MSG_ROW_STOP_THINKING	6000L					//	command to cancel search
#define CM_MSG_ACK_ROW_STOP_THINKING	6001L					//	acknowledge it
#define CM_MSG_CONSIDER_ROW			6002L					//	command to search a row
#define CM_MSG_CONSIDER_CELL		6003L					//	command to search a cell
#define CM_MSG_CELL_RESULT			6004L					//	message with result of cell
	
//	menu messages
// 	items on Game menu
#define AboutCMItem 				11001L					//	standard about box	
#define NewGameItem					11002L					//	restart game
#define TakeBackItem				11003L					//	take back last move
#define QuitItem					B_QUIT_REQUESTED		//	quit the game

//	items on Player 1 Menu
#define Player1HumanItem			12001L					//	define the red player to be human
#define Player1RandomItem			12002L					//	define the red player to be random
#define Player1SmartItem			12003L					//	define the red player to be smart
#define Player1AIItem				12004L					//	define the red player to be AI
#define Player1IsBillKocayItem		12005L					//	define the red player to be Bill Kocay
#define Player1IsAlbertEinsteinItem	12006L					//	define the red player to be Albert Einstein

//	items on Player 2 Menu
#define Player2HumanItem				13001L					//	define the blue player to be human
#define Player2RandomItem			13002L					//	define the blue player to be random
#define Player2SmartItem				13003L					//	define the blue player to be smart
#define Player2AIItem				13004L					//	define the blue player to be AI
#define Player2IsBillKocayItem		13005L					//	define the blue player to be Bill Kocay
#define Player2IsAlbertEinsteinItem	13006L					//	define the blue player to be Albert Einstein

//	items on Options Menu
#define OptionSoundToggleItem			14001L					//	toggle sound on/off
#define OptionFastDisplayItem			14002L					//	fast display mode
#define OptionMediumDisplayItem		14003L					//	medium display mode
#define OptionSlowDisplayItem			14004L					//	slow display mode
#define OptionWrapAroundItem			14005L					//	toggle wrap around mode
#define OptionRenderTorusItem			14006L					//	render torus

//	items on Help Menu
#define HelpGameRulesItem			15001L					//	show game rules
#define HelpMenuOptionsItem			15002L					//	show help on menu options

//	items on Priority Menu
#define LowPriorityItem				16000L					//	switch to low priority thought
#define NormalPriorityItem			16001L					//	switch to normal priority thought
#define DisplayPriorityItem			16002L					//	switch to display priority thought
#define UrgentDisplayPriorityItem		16003L 					//	switch to urgent display priority thought
#define RealTimeDisplayPriorityItem	16004L					//	switch to real time display priority thought
#define UrgentPriorityItem			16005L					//	switch to urgent priority thought
#define RealTimePriorityItem			16006L					//	switch to real time priority thought

#define HELP_STUFF_1 "Critical Mass (BeOS)\n\nBy Hamish Carr, 1998.\nhcarr@cs.ubc.ca\n\nA BeOS port of a game I wrote for a course\n\nHow does it work?\n\n"
#define HELP_STUFF_2 "Try blowing some stuff up and find out.  Or look at the Help menu for the game rules and options.\n\nSpecial thanks to J. Waschuk (j@intrinsix.ca) for the artwork."
#define HELP_STRING HELP_STUFF_1 HELP_STUFF_2
#define RULES_STUFF_1 "Critical Mass Rules\n\nThe players take turns building atoms by placing protons in the cells shown in the window.  Unfortunately, these atoms are highly unstable.  "
#define RULES_STUFF_2 "When the atom reaches critical mass, it explodes, throwing its protons into adjacent cells.  Each adjacent cell has one proton added, and is converted to the player's "
#define RULES_STUFF_3 " colour.  If the adjacent cell reaches critical mass as a result of the additional proton, it too explodes, in a chain reaction.  "
#define RULES_STUFF_4 "\n\nThe aim of the game is to exploit these chain reactions to wipe out your opponent's atoms."
#define RULES_STUFF_5 "\n\nThe edges of the window are reflective, so atoms at the edge reach critical mass sooner.  Critical mass for any cell is one proton for each of its neighbours."
#define RULES_STUFF_6 "Thus, critical mass in a corner square is 2, critical mass in an edge square is 3, and critical mass in a centre square is 4."
#define RULES_STRING RULES_STUFF_1 RULES_STUFF_2 RULES_STUFF_3 RULES_STUFF_4 RULES_STUFF_5 RULES_STUFF_6
#define MENUS_STUFF_1 "Critical Mass Menus\n\nMost of the menu options are self-explanatory.\n\nThere is a separate player menu for Red & Blue, so that you can play against the computer or "
#define MENUS_STUFF_2 "a friend.  As you go further down the list, the opponents get tougher.  On the Options menu, please remember that using low priority for the brain is recommended."
#define MENU_STRING MENUS_STUFF_1 MENUS_STUFF_2



