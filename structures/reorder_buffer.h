#pragma once

#include <queue>
#include "../instruction.h"

class ReorderBuffer
{
public:
	int entries;
	int freeEntries;
	std::queue<Instruction> table;
	ReorderBuffer(int entries);

	int insert(const Instruction& instr);
	bool isFull() const;
	void updateROB();
};