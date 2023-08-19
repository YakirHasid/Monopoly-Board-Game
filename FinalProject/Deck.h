#pragma once
#include <queue>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define MIN_VAL -350
#define MAX_VAL 350
#define THROW_CANNOT_COPY_DECKS throw "[EXECPTION] - Exception Thrown: Cannot copy decks, please review the code and implement correctly."

class Deck
{
	queue<int> cardsDeck;

public:
	Deck();

	int drawCard();
	int addCard();

	Deck operator=(Deck& a) { THROW_CANNOT_COPY_DECKS; };
	Deck(Deck& a) { THROW_CANNOT_COPY_DECKS; };
};
