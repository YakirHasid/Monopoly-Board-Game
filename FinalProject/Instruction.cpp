#include "Instruction.h"

/*
	constructor with a given string and enum
	uses initialization of base class Slot with the given string
*/
Instruction::Instruction(string slotName, instType inst) : Slot(slotName)
{
	this->inst = inst;
}

string Instruction::toString() const
{
	switch (this->inst)
	{
	case instType::GetTicket:
		return "Get Ticket";
		break;
	case instType::Jail:
		return "Jail";
		break;
	case instType::Start:
		return "Start";
		break;
	default:
		return "Unknown Instruction";
		break;
	}
}

/*
	implementation of pure virtual function print() lets us call the 'right' operator<< without using dynamic_cast or typeinfo.
	in runtime the print() function will be called here only when the object that is pointed into is Instruction.	
*/
void Instruction::print() const
{
	cout << *this;
}

ostream & operator<<(ostream & out, const Instruction & inst)
{
	out << "Instruction Type: " << inst.toString() << endl;
	out << "Instruction Name: " << inst.getSlotName() << endl;
	
	return out;
}
