#pragma once

#include <vector>

#include "../instruction.h"

class LoadStoreQueue
{
public:
	int maxEntries;
	std::vector<Instruction*> table;
	LoadStoreQueue(int entries);
	void Clear(Instruction* instr);
	bool IsFull();
	void Insert(Instruction* instruction);
};

class LoadStoreQueueAdder
{
public:
	int internalCycle;
	int cycleInEx;
	int op_code;
	int register_value;
	int offset;
	Instruction* instr;
	void Dispatch(Instruction* instruction);
	int Next();
	bool occupied;
	LoadStoreQueueAdder(int cycles_ex);
};

class MemoryUnit
{
public:
	int internalCycle;
	int cycleInEx;
	Instruction* instr;
	void Dispatch(Instruction* instruction);
	float Next();
	bool occupied;
	MemoryUnit(int cycles_ex);
};