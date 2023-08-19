#include "Deck.h"

/*
	empty constructor is used because default init for queue<int> is empty queue.
*/
Deck::Deck()
{
}

/*
	draws a card from the deck and returns its value
*/
int Deck::drawCard()
{
	// retrive the value of the card on the top of the deck.
	int val = cardsDeck.front();

	// pop the card from the deck.
	this->cardsDeck.pop();

	// insert the retrived card's value into the end of the deck.
	this->cardsDeck.push(val);

	// return the value of the card
	return val;
}

/*
	adds a random valued card to the deck.
	returns the value of the added card.
*/
int Deck::addCard()
{

	/*
	generate random number between -350 and 350
	Possible values: -350, -349, ... , 0, 1, 2, ... , 349, 350
	calculate the range between the edges (adding one to count the edge itself).
	add to the rand the min edge so it will shift the result to the correct place.
	*/
	int val = rand() % (MAX_VAL-MIN_VAL+1) + MIN_VAL;

	// pushes the new card inside the deck.
	this->cardsDeck.push(val);

	// returns the value of the added card.
	return val;
}
