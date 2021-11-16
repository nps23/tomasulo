#pragma once

#include <queue>
#include <map>
#include "../instruction.h"

class ReorderBuffer
{
public:
	int entries;
	int freeEntries;
	int id;
	std::queue<Instruction*> table;  // TODO change the calling functions to use pointers
	ReorderBuffer(int entries);

	int insert(const Instruction& instr);
	bool isFull() const;
	void updateROB();
};