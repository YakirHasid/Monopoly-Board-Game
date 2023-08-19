#include "gameEngine.h"

void main()
{
	// try to play a game, any exception will be catched below the try
	try
	{
		// create local game engine
		gameEngine gE;

		// build the gameboard
		gE.buildGameBoard();

		// initialize the game deck
		gE.initGameDeck();

		// initialize the players
		gE.initPlayers();

		// start the game
		gE.startGame();
	}
	// used for catching any const char* exception thrown when trying to play a game
	catch (const char* e)
	{
		// print the exception that was thrown
		cout << e;
	}

	/*
		can be reached after a successfully finished game,
		or after an exception was thrown during the game.
	*/
	cout << "Shutting down the program and clearing remaining memory..." << endl;
	system("pause");
}
