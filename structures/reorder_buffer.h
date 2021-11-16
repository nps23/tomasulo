#pragma once

#include <deque>
#include "../instruction.h"

class ReorderBuffer
{
public:
	int entries;
	std::deque<Instruction*> table;  // TODO change the calling functions to use pointers
	ReorderBuffer(int robEntries);

	void insert(Instruction& instr);
	void clear();
	bool isFull();
	bool isEmpty();
	void updateROB();
};
