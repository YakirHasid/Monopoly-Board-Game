#include "Player.h"

/*
	setter function for the place field
*/
void Player::setPlace(int place)
{
	this->place = place;
}

/*
	constructor with a given string
*/
Player::Player(string name)
{
	this->name = name;
	this->balance = START_BALANCE;
	this->place = START_PLACE;
	this->inJail = false;	
	this->assets.empty();
}

/*
	executes a transaction with a given amount.
	the amount can be:
	positive amount (credit)
	negative amount (charge)
	the function executes according to what was given.
*/
bool Player::transaction(int amount)
{
	// check if it's a positive transaction
	if (amount >= 0)
	{
		// add the transaction amount to the player's balance.
		cout << "Adding to the player's balance: " << amount << endl;
		this->balance+= amount;
		cout << "Player's new balance: " << this->balance << endl;
		// unpledge assets with the current balance.
		tryUnpledge();
		return true;
	}

	// check if the balance is enough to pay the negative amount
	if (this->balance >= abs(amount))
	{
		cout << "Subtracting from the player's balance: " << abs(amount) << endl;
		// amount is already negative, so addition is used instead of subtraction.
		this->balance += amount;
		cout << "Player's new balance: " << this->balance << endl;
		return true;
	}

	// balance is not enough, try to pledge assets in order to pay the amount.
	if (tryPledge(amount))
	{
		cout << "Subtracting from the player's balance: " << abs(amount) << endl;
		/* 
			managed to accumulate enough funds to pay the given amount.
			now the player needs to pay the given amount.
		*/		
		this->balance += amount;
		cout << "Player's new balance: " << this->balance << endl;
		return true;
	}
	else
	{
		/*
			did not manage to accumulate enough funds to pay the given amount.
			returns false and the player balance will now represent their total value,
			which can be transfered to another player after liquidation.
		*/
		return false;
	}
}

/*
	tries to unpledge assets with the player's balance.
	will pledge assets until balance does not allow it,
	or until there is no more assets left to unpledge.
*/
void Player::tryUnpledge()
{	
	vector<Asset *>::iterator iter = this->assets.begin();
	vector<Asset *>::iterator iterEnd = this->assets.end();
	int tempPrice;

	// iterate over assets array with additional condition that balance is positive
	for (; ((this->balance>0) && (iter != iterEnd)); ++iter)
	{
		// check if asset is pledged
		if ((*iter)->isPledged())
		{
			tempPrice = (*iter)->getUnpledgePrice();
			// check if balance is enough for unpledge
			if (this->balance >= tempPrice)
			{
				// update balance and unpledge the asset
				this->balance -= tempPrice;
				cout << "[UNPLEDGE ASSET] - Unpledging the following Asset:" << endl;
				plotSpacerDelim(delimType::Slot);
				cout << (*(*iter)) << endl;
				plotSpacerDelim(delimType::Slot);
				(*iter)->unpledgeAsset();				
			}
		}		
	}
}

/*
	tries to pledge assets from the player's assets in order to pay the given pledgeAmount.
*/
bool Player::tryPledge(int pledgeAmount)
{

	vector<Asset *>::iterator iter = this->assets.begin();
	vector<Asset *>::iterator iterEnd = this->assets.end();
	
	// store an absolute value of the pledgeAmount
	int absPledgeAmount = abs(pledgeAmount);
	
	// iterate over assets array with additional condition that balance is positive
	for (; ((this->balance < absPledgeAmount) && (iter != iterEnd)); ++iter)
	{
		// if the current asset is not pledged
		if (!(*iter)->isPledged())
		{
			// then pledge the asset and add the house price to the player's balance
			this->balance += (*iter)->getHousePrice();
			cout << "[PLEDGE ASSET] - Pledging the following Asset:" << endl;
			plotSpacerDelim(delimType::Slot);
			cout << (*(*iter)) << endl;
			plotSpacerDelim(delimType::Slot);
			(*iter)->pledgeAsset();
		}
	}

	/*
		return true if there is enough balance to pay the absolute value of pledgeAmount.
		else, return false.
	*/
	return this->balance >= absPledgeAmount;
}

/*
	check if the asset is available for purchase
	if so, check if the player has enough money in the balance
	if so, decrease the amount from balance and set the asset's owner as the player.
	else, can't buy
*/
bool Player::purchaseAsset(Asset * asset)
{
	
	// check if the asset is available for purchase
	if (asset->getOwner() != NULL)
	{
		THROW_ASSET_IS_ALREADY_OWNED;
	}

	// retrieve the asset price
	int assetPrice = asset->getHousePrice();

	// check if the player has enough money in the balance
	if (this->balance >= assetPrice)
	{
		// decrease the amount from balance and set the asset's owner as the player.
		this->balance -= assetPrice;
		asset->setOwner(this);
		this->addAsset(asset);

		// asset bought, return true
		return true;
	}

	// asset has not been bought, return false
	return false;
}

/*
	increases any pledged assets the player has by 1.
*/
void Player::incPledgedAssets()
{
	vector<Asset *>::iterator iter = this->assets.begin();
	vector<Asset *>::iterator iterEnd = this->assets.end();
	// loops through the player's assets
	for (; iter != iterEnd; ++iter)
	{
		// if an asset is pledged, increase the pledged years by 1.
		if ((*iter)->isPledged())
			(*iter)->incPledgeYears();
	}
}

/*
	adds an asset to the player's assets.
*/
void Player::addAsset(Asset * asset)
{
	// push back the asset pointer into the player's assets pointer vector
	this->assets.push_back(asset);
}

/*
	increases the player's balance with a given amount.
*/
void Player::incBalance(int amount)
{
	// check if the increment amount is negative
	if (amount < 0)
	{
		THROW_AMOUNT_INC_BELOW_ZERO;
	}

	// increment the balance with the amount
	this->balance += amount;
}

/*
	clears the player's balance.
	returns the amount that was cleard.
*/
int Player::clearBalance()
{	
	int tempBalance = this->balance;
	this->balance = 0;
	return tempBalance;
}

/*
	prints the details of a player.
*/
ostream & operator<<(ostream & out, const Player & p)
{
	out << "Player Name: " << p.name << endl;
	out << "Player Balance: " << p.balance << endl;
	out << "Player Place: " << p.place << endl;
	out << "Player's Assets: " << endl;
	if (p.assets.empty())
		out << "Player has no Assets." << endl;
	else
	{		
		vector<Asset *>::const_iterator iter = p.assets.begin();
		vector<Asset *>::const_iterator iterEnd = p.assets.end();
		/* 
			loop through the assets pointer vector,
			print each asset using the asset's ostream function.
		*/
		plotSpacerDelim(delimType::Slot);
		for (; iter != iterEnd; ++iter)
		{
			out << *(*iter);
			plotSpacerDelim(delimType::Slot);
		}

	}

	return out;
}
