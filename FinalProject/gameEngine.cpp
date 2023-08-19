#include "gameEngine.h"

// TODO(5): Complete gameEngine comments.

/*
	empty constructor is used because default init for vector, queue and deck works.	
*/
gameEngine::gameEngine()
{
	plotSpacerDelim(delimType::Main);
	cout << "Creating a Game With The Following Values:" << endl;
	cout << "Loop Pass Reward: " << LOOP_PASS_REWARD << endl;
	cout << "Interest Rate: " << INTEREST_RATE << endl;
	cout << "Dice Range: [" << MIN_DICE << " - " << MAX_DICE << "]" << endl;
	cout << "Minimum Number Of Players: " << MIN_NUM_OF_PLAYERS << endl;
	cout << "Start Place: " << START_PLACE << endl;
	cout << "Start Balance: " << START_BALANCE << endl;
	cout << "Deck Size: " << DECK_SIZE << endl;
	cout << "Deck Card Range: [" << MIN_VAL << " - " << MAX_VAL << "]" << endl;
	
}

/*
	builds the game board with the info retrieved from the board configuration file.
*/
void gameEngine::buildGameBoard()
{
	// open in file stream from BOARD_FILE_PATH path 
	ifstream in(BOARD_FILE_PATH, ios::in);

	// throw exception if the file didn't open
	if (!in.is_open())
		THROW_NO_BOARD_FILE;

	// create a buffer for a single line
	char buffer[MAX_LINE_SIZE];	

	// init values
	int index = 1;
	string buffStr;
	string delimiter = ",";
	int pos = 0;
	string token;	
	string name;
	Slot* newSlot;

	// holds the datas' field information
	string data[5];
	int dataIndex = 0;

	// temporary variables to send to constructor for each new Slot
	string cityName, slotName, price, rent;

	// pointer to the iterator of the beginning of the vector of Slot pointers.
	vector<Slot*>::iterator iter = this->gameBoard.begin();
	plotSpacerDelim(delimType::Main);
	cout << "Populating The Game Board's Slots:" << endl;

	// reads line by line from the in file stream
	while (in.getline(&buffer[0], (streamsize)(MAX_LINE_SIZE - 1)))
	{
		cout << "Slot " << index << ": ";
		cout << buffer << endl;
		buffStr = buffer;

		// break string to data fields as each field is divided by a delimiter.
		while ((pos = buffStr.find(delimiter)) != string::npos)
		{
			// put inside token a single data field
			token = buffStr.substr(0, pos);

			// insert the token into the right index
			data[dataIndex++] = token;

			// move into the next data field by clearing the already-handled field
			buffStr.erase(0, pos + delimiter.length());
		}

		// last field doesn't have a delimiter after it - which means it's the rest of what left from the buffer
		data[dataIndex] = buffStr;
		newSlot = this->parseData(data, dataIndex);
		dataIndex = 0;
		this->gameBoard.push_back(newSlot);

		index++;
	}

	cout << "Building Game Board Complete." << endl;

	// closing file as reading has been completed.
	in.close();

	// print to screen all of the slots directly from the vector of base class
	cout << "Printing Board..." << endl;
	plotSpacerDelim(delimType::Slot);
	iter = this->gameBoard.begin();
	vector<Slot*>::iterator iterEnd = this->gameBoard.end();
	index = 1;
	for (; iter != iterEnd; ++iter)
	{
		cout << "Slot #" << index << ":" << endl;
		index++;
		(*iter)->print();
		plotSpacerDelim(delimType::Slot);
	}

}

/*
	initialize the players information by asking the user for input
*/
void gameEngine::initPlayers()
{
	plotSpacerDelim(delimType::Main);
	cout << "Initializing The Game Board's Players and Players Queue:" << endl;
	int numOfPlayers;

	// makes sure the user will only type a number of players that is above the defined minimum.
	do
	{
		cout << "Please enter the number of players: ";
		cin >> numOfPlayers;		
		if (numOfPlayers < MIN_NUM_OF_PLAYERS)
			cout << "Minimum number of players is: " << MIN_NUM_OF_PLAYERS << endl;
	} while (numOfPlayers < MIN_NUM_OF_PLAYERS);

	// asks for each player a name
	string name;
	Player* newPlayer;
	for (int i = 0; i < numOfPlayers; i++)
	{
		cout << "[PLAYER " << (i+1) << "]: Please enter your name: ";
		cin >> name;

		// create a new player with the given name
		newPlayer = new Player(name);
		this->playerQueue.push(newPlayer);
	}
}

/*
	initialize the game deck with a defined deck size.
*/
void gameEngine::initGameDeck()
{
	plotSpacerDelim(delimType::Main);
	cout << "Initializing The Game Board's Deck:" << endl;
	int val;
	// add DECK_SIZE cards to the game deck
	for (int i = 0; i < DECK_SIZE; i++)
	{
		// call a helper function that adds a random valued card to the deck.
		val = this->gameDeck.addCard();
		cout << "Added Card #" << (i + 1) << ": " << val << endl;
	}
}

/*
	starts the game.
	the function will finish once the game is over.
*/
void gameEngine::startGame()
{
	plotSpacerDelim(delimType::Main);
	cout << "Starts The Game:" << endl;
	// randomize seed based on time
	srand(time(NULL));

	Player* player = NULL;

	/* 
	game logic needs at least 2 players in order to start the game.
	
	*/
	if (this->playerQueue.size() < MIN_NUM_OF_PLAYERS)
		THROW_NUM_OF_PLAYERS_BELOW_MIN;

	while (playerQueue.size() > 1)
	{
		// peek the queue to get the player's pointer.
		player = this->playerQueue.front();

		// pop the player from the queue as we are currently handling them.
		this->playerQueue.pop();

		plotSpacerDelim(delimType::Main);
		plotSpacerDelim(delimType::Sub);
		cout << "[Current Turn Of]:" << endl;
		cout << (*player);
		plotSpacerDelim(delimType::Sub);

		// if the player is currently in jail, use this turn to free them from jail.
		if (player->isInJail())
		{			
			// freeing the player from jail
			player->freeFromJail();
			// adding the player back to the queue
			this->playerQueue.push(player);
			cout << "Used current turn to free player '" << player->getName() << "' from jail." << endl;
		}			
		// if the player is not in jail, allow the player to play its turn.
		else if (turn(player))
		{
			// adding the player back to the queue after their turn is over and they didn't lose or quit
			this->playerQueue.push(player);
		}
		/*
		if the turn function returned false, the player chose to quit the game or they've lost the game.
		so a false return means the player's data is ready to be released.
		*/
		else
		{
			plotSpacerDelim(delimType::Rem);
			cout << "Player " << player->getName() << " has been eliminated from the game." << endl;
			plotSpacerDelim(delimType::Rem);
			this->releasePlayer(player);
			delete player;
		}			
	}

	/*
		only one player left, which is the winner
		so dequeue the winner
	*/
	player = this->playerQueue.front();
	this->playerQueue.pop();

	// announce the winner
	this->congratsWinner(player);

	//release the data of the last player.
	this->releasePlayer(player);
	delete player;
}

/*
	handles a game turn of a given player
*/
bool gameEngine::turn(Player * player)
{
	// makes sure a valid response is given and handles the response.
	char response;
	do
	{
		cout << "[PLAYER " << player->name << "]: Do you want to continue playing? y/n (type y to continue):";
		cin >> response;
		// incase the player chose to quit, simply return false as the turn has ended with a request from the player to exit the game.
		if (response == CHOICE_QUIT)
		{
			return false;
		}

		// makes sure a valid response is given
		if (response != CHOICE_CONTINUE)
		{
			cout << "Invalid option selected." << endl;
		}
	} while (response != CHOICE_CONTINUE);

	// throws the dice
	int dice = this->throwDice();

	// executes the move, will forward the response from executeMove(...) to the caller of turn(...)
	return this->executeMove(player, dice);
}

/*
	releases all the assets associated with the given player
*/
void gameEngine::releasePlayer(Player * player)
{
	vector<Asset *> assets = player->getAssets();
	vector<Asset *>::iterator iter = assets.begin();
	vector<Asset *>::iterator iterEnd = assets.end();

	// for each asset the player owns, set the owner to be NULL.
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->setOwner(NULL);
	}
}

/*
	Advance the player on the board by the given dice result.
	Handles the advance on board part of the turn
*/
bool gameEngine::advPlayer(Player * player, int dice) const
{
	// advance the player's place with the dice result
	// return false if it didn't complete a loop
	player->place += dice;
	if (player->place <= this->gameBoard.size())
		return false;

	// corrects place according to overlap of board size.
	player->place = player->place%(this->gameBoard.size() + 1) + 1;

	// return true so caller will know that a loop has been completed.
	return true;
}

/*
	Executes the move action with a given player and by the given dice result
	Handles all aspects of the moving part of the turn
*/
bool gameEngine::executeMove(Player * player, int dice)
{
	// calls the helper function advPlayer to advance the player on the board.
	if (this->advPlayer(player, dice))
	{
		cout << "Board loop has occured, awarding player '" << player->getName() << "' " << LOOP_PASS_REWARD << " and increasing pledged assets years by 1." << endl;
		player->incBalance(LOOP_PASS_REWARD);
		player->incPledgedAssets();

	}		

	// retrieve the player's current slot
	Slot* currentSlot = this->retSlot(player);
	cout << "Landed on Slot #" << player->place << ":" << endl;

	plotSpacerDelim(delimType::Slot);
	currentSlot->print();
	plotSpacerDelim(delimType::Slot);

	// creates a new pointer of the derived class after checking what is the derived class
	// executes a helper function associated with the derived slot type.
	if (typeid(*currentSlot) == typeid(Instruction))
	{
		Instruction* dervFromSlot = dynamic_cast<Instruction*>(currentSlot);
		return this->executeInst(player, dervFromSlot);
	}
	else if (typeid(*currentSlot) == typeid(Asset))
	{
		Asset* dervFromSlot = dynamic_cast<Asset*>(currentSlot);
		return executeAsset(player, dervFromSlot);
	}
	else
	{
		THROW_UNKNOWN_DERIVED_FROM_SLOT;
	}

	// executed move successfully, player didn't lose.
	return true;
}

/*
	Handles a given player executing a given instruction.
*/
bool gameEngine::executeInst(Player * player, Instruction * inst)
{
	int amount;
	switch (inst->getInst())
	{
	case instType::GetTicket:
		// draw a ticket from the deck		
		amount = this->gameDeck.drawCard();
		cout << "Drawn a ticket with the value of: " << amount << endl;
		// activate the card amount, return the bool result of operation.
		return player->transaction(amount);
		break;
	case instType::Jail:
		cout << "Going to jail..." << endl;
		player->putInJail();
		break;
	case instType::Start:
		// no need to add the money here, as the player looped already and got the reward.
		// it's possible to get "Double Cash" for "Finished Loops Right On Start"
		// and give extra reward cash here.
		break;
	}

	// executed instruction successfully, player didn't lose.
	return true;
}

/*
	Handles a given player executing a given asset.
*/
bool gameEngine::executeAsset(Player * player, Asset * asset)
{
	char answer;

	// the asset has an owner and it's not the player.
	if (asset->getOwner())
	{
		if (asset->getOwner() != player)
		{
			int price = asset->getRent();
			Player* owner = asset->getOwner();
			cout << "Trying to pay rent from player '" << player->getName() << "' ";
			cout << "to player '" << owner->getName() << "'." << endl;

			// check if the player can pay the rent price
			if (player->transaction(-price))
			{
				/*
					the player's balance has already been decremented.
					if the asset is not leased - we will forward that money the owner's balance.
					else if the asset is leased - we will 'return the money to the bank' (will not give it to any player).
				*/				
				if (!asset->isPledged())
				{
					cout << "Transfering to '" << owner->getName() << "' " << price << " from '" << player->getName() << "'" << endl;
					owner->incBalance(price);
				}
				else
				{
					cout << "Transfering to the bank " << price << " from '" << player->getName() << "'" << endl;
				}

			}
			/*
				the player did not have enough balance to pay the rent, which means the player lost.
				we will forward the remaining balance of player's to the owner as compensation.
			*/			
			else
			{
				cout << "Insufficient funds for '" << player->getName() << "' to pay rent." << endl;
				cout << "Transfering to '" << owner->getName() << "' all of '" << player->getName() << "' balance and clearing them from the game." << endl;
				owner->incBalance(player->clearBalance());
				return false;
			}

		}
		else
			cout << "You've landed on an Asset that you own." << endl;
	}
	// the asset does not have an owner
	else
	{
		cout << "Would you like to purchase the Asset? y/n: ";
		cin >> answer;
		if (answer == 'y')
		{
			cout << "Trying to buy asset..." << endl;
			if (player->purchaseAsset(asset))
				cout << "Asset purchased successfully." << endl;
			else
				cout << "Failed to purchase the asset (insufficient funds)." << endl;
		}
	}

	// executed asset successfully, player didn't lose.
	return true;
}

/*
	Handles a dice throwing, returns the result of what the dice landed on.
*/
int gameEngine::throwDice() const
{

	cout << "Throwing dice..." << endl;

	/*
		generate random number between 1 and 6
		Possible values: 1, 2, 3, 4, 5, 6
		calculate the range between the edges (adding one to count the edge itself).
		add to the rand the min edge so it will shift the result to the correct place.
	*/
	int val = rand() % (MAX_DICE - MIN_DICE + 1) + MIN_DICE;

	cout << "Dice result: " << val << endl;

	return val;
}

/*
	prints a congrats to the given player, which is declared a winner
*/
void gameEngine::congratsWinner(Player* player) const
{
	plotSpacerDelim(delimType::Main);
	cout << "[GAME FINISHED]" << endl;
	plotSpacerDelim(delimType::Main);
	plotSpacerDelim(delimType::Sub);
	plotSpacerDelim(delimType::Sub);
	plotSpacerDelim(delimType::Sub);
	cout << "CONGRATS '" << player->getName() << "' ON WINNING!" << endl;
	plotSpacerDelim(delimType::Sub);
	plotSpacerDelim(delimType::Sub);
	plotSpacerDelim(delimType::Sub);
}

/*
	retrieves the slot pointer the given player currently stands on
*/
Slot * gameEngine::retSlot(Player * player) const
{
	if (player->place < 1 || player->place > this->gameBoard.size())
		THROW_POS_OUT_OF_BOUNDS;

	return this->gameBoard[player->place - 1];
}

/*
	Parses the data of the string array.
	Creates a Slot pointer with new derived class based on the information inside the string array.
*/
Slot * gameEngine::parseData(string * arr, int size) const
{
	Slot* newSlot = NULL;
	switch (arr[0][0])
	{
	case SLOT_TYPE_INSTRUCTION:
		switch (arr[1][0])
		{
		case INST_TYPE_JAIL:
			newSlot = new Instruction(arr[2], instType::Jail);
			break;
		case INST_TYPE_START:
			newSlot = new Instruction(arr[2], instType::Start);
			break;
		case INST_TYPE_TICKET:
			newSlot = new Instruction(arr[2], instType::GetTicket);
			break;
		default:
			THROW_UNKNOWN_SLOT_TYPE;
			break;
		}
		break;
	case SLOT_TYPE_PROPERTY:
		newSlot = new Asset(arr[1], arr[2], stoi(arr[3]), stoi(arr[4]));
		break;
	default:
		THROW_PARSE_UNKNOWN_SLOT;
		break;
	}

	return newSlot;
}
