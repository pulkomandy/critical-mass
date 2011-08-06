//	Critical Mass be
//	31/10/1998
//	Hamish Carr
//
//	CMassMenuBar.cpp

#include "CMassMenuBar.h"
#include <MenuItem.h>
#include <Application.h>

CMassMenuBar::CMassMenuBar(BRect frame, char *name)								//	constructor
	: BMenuBar(frame, name)													//	call inherited constructor
	{
	BMenu *aMenu;															//	used for adding menu items
	BMenuItem	*theMenuItem;													//	used for creating menu items

	//	Game menu
	aMenu = new BMenu("Game", B_ITEMS_IN_COLUMN);								//	initialize a menu
	theMenuItem = new BMenuItem("About Critical Mass", new BMessage(AboutCMItem), 'A', NULL);
	aMenu->AddItem(theMenuItem);
	aMenu->AddSeparatorItem();												//	add a separator
	theMenuItem = new BMenuItem("New Game", new BMessage(NewGameItem), 'N', NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Take Back Move", new BMessage(TakeBackItem), 'T', NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Quit", new BMessage(QuitItem), 'Q', NULL);
	aMenu->AddItem(theMenuItem);
	AddItem(aMenu);														//	add this menu to the menu bar
	aMenu->SetTargetForItems(be_app);											//	be paranoid & tell the last menu as well

	//	Player 1 menu
	aMenu = new BMenu("Red", B_ITEMS_IN_COLUMN);									//	initialize a menu
	theMenuItem = new BMenuItem("Human", new BMessage(Player1HumanItem), NULL, NULL);
	theMenuItem->SetMarked(true);
	aMenu->AddItem(theMenuItem);
	aMenu->AddSeparatorItem();												//	add a separator
	theMenuItem = new BMenuItem("Random", new BMessage(Player1RandomItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Smart", new BMessage(Player1SmartItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("A.I.", new BMessage(Player1AIItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Bill Kocay", new BMessage(Player1IsBillKocayItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Albert Einstein", new BMessage(Player1IsAlbertEinsteinItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	AddItem(aMenu);														//	add this menu to the menu bar
	aMenu->SetTargetForItems(be_app);											//	be paranoid & tell the last menu as well

	//	Player 2 menu
	aMenu = new BMenu("Blue", B_ITEMS_IN_COLUMN);								//	initialize a menu
	theMenuItem = new BMenuItem("Human", new BMessage(Player2HumanItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	aMenu->AddSeparatorItem();												//	add a separator
	theMenuItem = new BMenuItem("Random", new BMessage(Player2RandomItem), NULL, NULL);
	theMenuItem->SetMarked(true);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Smart", new BMessage(Player2SmartItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("A.I.", new BMessage(Player2AIItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Bill Kocay", new BMessage(Player2IsBillKocayItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Albert Einstein", new BMessage(Player2IsAlbertEinsteinItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	AddItem(aMenu);														//	add this menu to the menu bar
	aMenu->SetTargetForItems(be_app);											//	be paranoid & tell the last menu as well

	//	Options menu
	aMenu = new BMenu("Options", B_ITEMS_IN_COLUMN);								//	initialize a menu
	theMenuItem = new BMenuItem("Sound", new BMessage(OptionSoundToggleItem), 'S', NULL);
	theMenuItem->SetMarked(true);
	aMenu->AddItem(theMenuItem);
	aMenu->AddSeparatorItem();												//	add a separator
	theMenuItem = new BMenuItem("Fast Display", new BMessage(OptionFastDisplayItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Medium Display", new BMessage(OptionMediumDisplayItem), NULL, NULL);
	theMenuItem->SetMarked(true);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Slow Display", new BMessage(OptionSlowDisplayItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	aMenu->AddSeparatorItem();												//	add a separator
	theMenuItem = new BMenuItem("Wrap Around", new BMessage(OptionWrapAroundItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	AddItem(aMenu);														//	add this menu to the menu bar
	aMenu->SetTargetForItems(be_app);											//	be paranoid & tell the last menu as well

	torusItem = new BMenuItem("Render Torus", new BMessage(OptionRenderTorusItem), NULL, NULL);
	torusItem->SetTarget(be_app);												//	target it properly

	//	Thought Speed menu
	aMenu = new BMenu("Priority", B_ITEMS_IN_COLUMN);								//	initialize a menu
	theMenuItem = new BMenuItem("Low Priority", new BMessage(LowPriorityItem), NULL, NULL);
	theMenuItem->SetMarked(true);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Normal Priority", new BMessage(NormalPriorityItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Display Priority", new BMessage(DisplayPriorityItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Urgent Display Priority", new BMessage(UrgentDisplayPriorityItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Real Time Display Priority", new BMessage(RealTimeDisplayPriorityItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Urgent Priority", new BMessage(UrgentPriorityItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Hog Mode", new BMessage(RealTimePriorityItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	AddItem(aMenu);														//	add this menu to the menu bar
	aMenu->SetTargetForItems(be_app);											//	be paranoid & tell the last menu as well

	//	Help menu
	aMenu = new BMenu("Help", B_ITEMS_IN_COLUMN);								//	initialize a menu
	theMenuItem = new BMenuItem("About Critical Mass", new BMessage(AboutCMItem), 'A', NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Game Rules", new BMessage(HelpGameRulesItem), 'H', NULL);
	aMenu->AddItem(theMenuItem);
	theMenuItem = new BMenuItem("Game Menus", new BMessage(HelpMenuOptionsItem), NULL, NULL);
	aMenu->AddItem(theMenuItem);
	AddItem(aMenu);														//	add this menu to the menu bar
	aMenu->SetTargetForItems(be_app);											//	be paranoid & tell the last menu as well
	
	} // end of CMassMenuBar constructor

void CMassMenuBar::MessageReceived(BMessage *theEvent)								//	reacts to messages
	{
	long i;																//	loop index
	BMenuItem *theItem;														//	to retrieve the item (if it exists)
	switch (theEvent->what)													//	just a big switch statement
		{
		case CM_MARK_SOUND_ON:												//	tells window to mark sound as being on
			theItem = FindItem(OptionSoundToggleItem);							//	retrieve the correct item
			theItem->SetMarked(true);										//	switch it on
			break;
		case CM_MARK_SOUND_OFF:												//	tells window to mark sound as being off
			theItem = FindItem(OptionSoundToggleItem);							//	retrieve the correct item
			theItem->SetMarked(false);										//	switch it off
			break;
		case Player1HumanItem:												//	define the red player to be human
		case Player1RandomItem:												//	define the red player to be random
		case Player1SmartItem:												//	define the red player to be smart
		case Player1AIItem:													//	define the red player to be AI
		case Player1IsBillKocayItem:											//	define the red player to be Bill Kocay
		case Player1IsAlbertEinsteinItem:										//	define the red player to be Albert Einstein
			for (i = Player1HumanItem; i <= Player1IsAlbertEinsteinItem; i++)		//	walk through all of them
				{
				theItem = FindItem(i);										//	retrieve the item
				if (i == theEvent->what)										//	if it is the one we want
					theItem->SetMarked(true);								//	switch it on
				else
					theItem->SetMarked(false);								//	switch it off
				} // end of loop through player 1 items
			break;
			
		case Player2HumanItem:												//	define the blue player to be human
		case Player2RandomItem:												//	define the blue player to be random
		case Player2SmartItem:												//	define the blue player to be smart
		case Player2AIItem:													//	define the blue player to be AI
		case Player2IsBillKocayItem:											//	define the blue player to be Bill Kocay
		case Player2IsAlbertEinsteinItem:										//	define the blue player to be Albert Einstein
			for (i = Player2HumanItem; i <= Player2IsAlbertEinsteinItem; i++)		//	walk through all of them
				{
				theItem = FindItem(i);										//	retrieve the item
				if (i == theEvent->what)										//	if it is the one we want
					theItem->SetMarked(true);								//	switch it on
				else
					theItem->SetMarked(false);								//	switch it off
				} // end of loop through player 2 items
			break;
		case OptionFastDisplayItem:											//	fast display mode
		case OptionMediumDisplayItem:											//	medium display mode
		case OptionSlowDisplayItem:											//	slow display mode
			for (long i = OptionFastDisplayItem; i <= OptionSlowDisplayItem; i++)		//	walk through them
				{
				theItem = FindItem(i);										//	retrieve the item
				if (i == theEvent->what)										//	if it is the one we want
					theItem->SetMarked(true);								//	switch it on
				else
					theItem->SetMarked(false);								//	switch it off
				} // end of loop through speed items
			break;
		case LowPriorityItem:												//	switch to low priority thought
		case NormalPriorityItem:												//	switch to normal priority thought
		case DisplayPriorityItem:											//	switch to display priority thought
		case UrgentDisplayPriorityItem:										//	switch to urgent display priority thought
		case RealTimeDisplayPriorityItem:										//	switch to real time display priority thought
		case UrgentPriorityItem:												//	switch to urgent priority thought
		case RealTimePriorityItem:											//	switch to real time priority thought
			for (long i = LowPriorityItem; i <= RealTimePriorityItem; i++)			//	walk through them
				{
				theItem = FindItem(i);										//	retrieve the item
				if (i == theEvent->what)										//	if it is the one we want
					theItem->SetMarked(true);								//	switch it on
				else
					theItem->SetMarked(false);								//	switch it off
				} // end of loop through speed items
			break;
		case OptionWrapAroundItem:											//	toggle wrap-around mode
			theItem = FindItem(theEvent->what);								//	retrieve the item
			if (theItem->IsMarked())											//	it's already marked
				{
				theItem->SetMarked(false);									//	unmark the item
				theItem->Menu()->RemoveItem(torusItem);							//	remove the torus rendering option
				} // end of marked case
			else
				{
				theItem->SetMarked(true);									//	mark the torus item on
				theItem->Menu()->AddItem(torusItem);							//	add it to the menu
				} // end of unmarked case
			break;
		case OptionRenderTorusItem:											//	toggle OpenGL mode
			torusItem->SetMarked(!torusItem->IsMarked());						//	revise the mark
			break;
		default:															//	if we don't recognize it
			BMenuBar::MessageReceived(theEvent);								//	pass it on to inherited method
		} // end of switch on theEvent->what
	} // end of MessageReceived()
