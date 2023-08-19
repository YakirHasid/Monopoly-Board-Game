#pragma once
#include <string>
using namespace std;

#define THROW_CANNOT_CREATE_EMPTY_SLOT throw "[EXECPTION] - Exception Thrown: Cannot create empty slots."

class Slot
{
	string slotName;
protected:
	Slot(string slotName);
	Slot() { THROW_CANNOT_CREATE_EMPTY_SLOT; };
public:
	void setSlotName(string slotName);
	string getSlotName() const { return this->slotName; };
	virtual void print() const = 0;
	virtual ~Slot();
};
