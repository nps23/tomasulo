#pragma once

#include <vector>

#include "../instruction.h"

class cdb
{
public:
	cdb();
	std::vector<Instruction*> buffer;
	bool occupied;
	void insert(Instruction* instr);
	void clear(Instruction* instr);
};