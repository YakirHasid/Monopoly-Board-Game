#pragma once
#include "Asset.h"
#include "PlotConfiguration.h"
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

#define START_PLACE 1
#define START_BALANCE 350

#define THROW_ASSET_IS_ALREADY_OWNED throw "[EXECPTION] - Exception Thrown: Asset is already owned by another Player."
#define THROW_AMOUNT_INC_BELOW_ZERO throw "[EXECPTION] - Exception Thrown: Trying to increment the player's balance with a negative value."
#define THROW_CANNOT_COPY_PLAYERS throw "[EXECPTION] - Exception Thrown: Cannot copy players, please review the code and implement correctly."

class Asset;
class Player
{
	string name;
	int balance;
	int place;
	vector<Asset *> assets;
	bool inJail;

public:
	void setPlace(int place);	
	vector<Asset *> getAssets() const { return this->assets; }
	string getName() const { return this->name; }
	friend class gameEngine;	
	Player(string name);

	bool transaction(int amount);
	void tryUnpledge();
	bool tryPledge(int pledgeAmount);
	bool purchaseAsset(Asset * asset);
	void incPledgedAssets();
	void addAsset(Asset* asset);

	void incBalance(int amount);	
	int clearBalance();

	void putInJail() { this->inJail = true; };
	void freeFromJail() { this->inJail = false; };
	bool isInJail() const { return inJail; };

	friend ostream& operator<<(ostream& out, const Player& p);
	Player operator=(Player& a) { THROW_CANNOT_COPY_PLAYERS; };
};
