#pragma once

#include <deque>
#include <vector>

#include "../instruction.h"

class ReorderBuffer
{
public:
	int entries;
	bool hasCommited{ false };
	std::vector<Instruction*> table;

	ReorderBuffer(int robEntries);
	void insert(Instruction* instr);
	void clear(Instruction* instr);
	bool isFull();
	bool isEmpty();
	void updateROB();
};
