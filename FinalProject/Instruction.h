#pragma once
#include "Slot.h"
#include <iostream>
enum instType {Jail, Start, GetTicket};

class Instruction : public Slot
{	
	instType inst;
public:
	Instruction(string slotName, instType inst);
	instType getInst() const { return this->inst; }	
	string toString() const;
	 
	friend ostream& operator<<(ostream& out, const Instruction& inst);

	virtual void print() const;
};
