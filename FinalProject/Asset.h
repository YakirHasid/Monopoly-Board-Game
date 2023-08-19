#pragma once
#include "Slot.h"
#include "Player.h"
#include "PlotConfiguration.h"
#include <iostream>
#include <string>
using namespace std;

#define INTEREST_RATE 0.5
#define ASSET_NO_OWNER NULL
#define ASSET_NOT_PLEDGED 0
#define ASSET_BEGIN_PLEDGE 1
#define THROW_CANNOT_COPY_ASSETS throw "[EXECPTION] - Exception Thrown: Cannot copy assets, please review the code and implement correctly."

class Player;
class Asset : public Slot
{
	string cityName;
	int housePrice;
	int rent;
	Player * owner;
	int pledgeYears;
	
public:	
	Asset(string cityName, string slotName, int housePrice, int rent);	

	string getCityName() const { return this->cityName; };
	int getHousePrice() const { return this->housePrice; };
	int getRent() const { return this->rent; };
	Player* getOwner() const { return this->owner; };
	int getPledgeYears() const { return this->pledgeYears; };

	void setOwner(Player * player);
	void setPledgeYears(int pledgeYears);	

	void incPledgeYears();
	void unpledgeAsset();
	void pledgeAsset();
	bool isPledged() const;

	virtual void print() const;

	friend ostream& operator<<(ostream& out, const Asset& a);
	Asset operator=(Asset& a) { THROW_CANNOT_COPY_ASSETS; };
	Asset (Asset& a) { THROW_CANNOT_COPY_ASSETS; };

	int getUnpledgePrice() const;
	~Asset();
};
