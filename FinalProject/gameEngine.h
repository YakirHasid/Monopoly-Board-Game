#pragma once
#include "Asset.h"
#include "Player.h"
#include "Deck.h"
#include "Slot.h"
#include "Instruction.h"
#include "PlotConfiguration.h"
#include <string>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define LOOP_PASS_REWARD 350
#define DECK_SIZE 30
#define BOARD_FILE_PATH "board.txt"
#define MAX_LINE_SIZE 100
#define BOARD_FILE_DELIMITER ','

#define SLOT_TYPE_INSTRUCTION 'I'
#define SLOT_TYPE_PROPERTY 'P'

#define INST_TYPE_JAIL 'J'
#define INST_TYPE_START 'S'
#define INST_TYPE_TICKET 'T'

#define CHOICE_CONTINUE 'y'
#define CHOICE_QUIT 'n'

#define MIN_DICE 1
#define MAX_DICE 6

#define MIN_NUM_OF_PLAYERS 2 // can not be below 2, this sets the min required number of players

#define THROW_NO_BOARD_FILE throw "[EXECPTION] - Exception Thrown: The Board Configuration File could not have been retreived, please make sure the file is in the right path and try again."
#define THROW_POS_OUT_OF_BOUNDS throw "[EXECPTION] - Exception Thrown: Player position is out of bounds."
#define THROW_UNKNOWN_DERIVED_FROM_SLOT throw "[EXECPTION] - Exception Thrown: Couldn't find the derived class of a Slot object."
#define THROW_NUM_OF_PLAYERS_BELOW_MIN throw "[EXECPTION] - Exception Thrown: Number of players is incorrect, needs the minimum number of players in order to start."
#define THROW_UNKNOWN_SLOT_TYPE throw "[EXECPTION] - Exception Thrown: Received an unknown Slot Type from the board configuration."
#define THROW_CANNOT_COPY_GAME_ENGINES throw "[EXECPTION] - Exception Thrown: Cannot copy game engines, please review the code and implement correctly."
#define THROW_PARSE_UNKNOWN_SLOT throw "[EXECPTION] - Exception Thrown: Trying to Parse an unknown slot type."

class gameEngine
{
	vector<Slot *> gameBoard;
	queue<Player *> playerQueue;
	Deck gameDeck;

public:
	gameEngine();

	void buildGameBoard();
	void initPlayers();
	void initGameDeck();
	void startGame();
	bool turn(Player* player);
private:
	void releasePlayer(Player* player);
	bool advPlayer(Player* player, int dice) const;
	bool executeMove(Player* player, int dice);
	bool executeInst(Player* player, Instruction* inst);
	bool executeAsset(Player* player, Asset* asset);
	int throwDice() const;
	void congratsWinner(Player* player) const;
	Slot* retSlot(Player* player) const;

	Slot* parseData(string* arr, int size) const;

	gameEngine operator=(gameEngine& a) { THROW_CANNOT_COPY_GAME_ENGINES; };
	gameEngine(gameEngine& a) { THROW_CANNOT_COPY_GAME_ENGINES; };
};
