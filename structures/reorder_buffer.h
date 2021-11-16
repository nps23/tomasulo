#pragma once

#include <deque>
#include "../instruction.h"

class ReorderBuffer
{
public:
	int entries;
	std::deque<Instruction*> table;  // TODO change the calling functions to use pointers
	ReorderBuffer(int entries);

	void insert(Instruction& instr);
	void clear(const Instruction& instr);
	bool isFull() const;
	bool isEmpty() const;
	void updateROB();
};