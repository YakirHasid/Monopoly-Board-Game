#include "Asset.h"

/*
	constructor with a given string, string, int and int
	uses initialization of base class Slot with the second given string
*/
Asset::Asset(string cityName, string slotName, int housePrice, int rent) : Slot(slotName)
{
	this->cityName = cityName;
	this->housePrice = housePrice;
	this->rent = rent;
	this->owner = ASSET_NO_OWNER;
	this->pledgeYears = ASSET_NOT_PLEDGED;
}

/*
	sets the retrieved player as the current owner of the asset
*/
void Asset::setOwner(Player * player)
{
	this->owner = player;
}

/*
	sets the retrieved pledge years as the current pledge years of the asset
*/
void Asset::setPledgeYears(int pledgeYears)
{
	this->pledgeYears = pledgeYears;
}

/*
	increments the current pledge years of the asset by 1.
*/
void Asset::incPledgeYears()
{
	this->pledgeYears++;
}

/*
	unpledges the current asset.
*/
void Asset::unpledgeAsset()
{
	this->pledgeYears = ASSET_NOT_PLEDGED;
}

/*
	pledges the current asset.
*/
void Asset::pledgeAsset()
{
	this->pledgeYears = ASSET_BEGIN_PLEDGE;
}

/*
	returns true if the asset is pledge,
	else returns false
*/
bool Asset::isPledged() const
{
	return this->pledgeYears!= ASSET_NOT_PLEDGED;
}

/*
	implementation of pure virtual function print() lets us call the 'right' operator<< without using dynamic_cast or typeinfo.
	in runtime the print() function will be called here only when the object that is pointed into is Asset.
*/
void Asset::print() const
{
	cout << *this;
}

/*
	calculates the unpledge price with a formula.
	base price = housePrice
	increment = housePrice * pledgeYears * INTEREST_RATE
	returns the unpledge price.
*/
int Asset::getUnpledgePrice() const
{
	/*
	calculates the unpledge price via the formula:
	adds to the house price the following:
	calculate the house price times the current pledge years times an constant interest rate.
	which means base price + increment
	*/
	return (int)(this->housePrice + (this->housePrice * this->pledgeYears * INTEREST_RATE));
}

Asset::~Asset()
{
	/* 
		even though this->owner is a pointer.
		data managment is not handled by this class.
	*/
}

/*
	print operator << for class Asset.
	used for easy printing of an asset's details.
*/
ostream & operator<<(ostream & out, const Asset & a)
{
	out << "City Name: " << a.cityName << endl;
	out << "Property Name: " << a.getSlotName() << endl;
	out << "House Price: " << a.housePrice << endl;
	out << "Rent: " << a.rent << endl;	
	if(a.owner)
		out << "Owner: " << a.getOwner()->getName() << endl;
	else
		out << "Asset is not owned." << endl;
	out << "Pledge Years: " << a.pledgeYears << endl;

	return out;
}
