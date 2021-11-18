#pragma once

#include <vector>

#include "../instruction.h"

class cdb
{
public:
	cdb();
	std::vector<Instruction*> buffer;
	bool occupied;
	bool isEmpty();
	void insert(Instruction* instr);
	void clear(Instruction* instr);
};