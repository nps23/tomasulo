#pragma once

#include <deque>

#include "../instruction.h"

class ReorderBuffer
{
public:
	int entries;
	bool hasCommited{ false };
	std::deque<Instruction*> table;

	ReorderBuffer(int robEntries);
	void insert(Instruction* instr);
	void pop();
	void clear(Instruction* instr);
	bool isFull();
	bool isEmpty();
	void updateROB();
};
