#include <vector>
#include <iostream>

#include "load_store_queue.h"
#include "../dataHoldingStructures.h"

extern cpuMemory mainMem;

LoadStoreQueue::LoadStoreQueue(int entries)
{
	maxEntries = entries;
	table.resize(0);
}

void LoadStoreQueue::Clear(Instruction* instr)
{
	table.erase(std::remove(table.begin(), table.end(), instr));
}

bool LoadStoreQueue::IsFull()
{
	return table.size() == maxEntries;
}

void LoadStoreQueue::Insert(Instruction* instr)
{
	if (table.size() == maxEntries)
	{
		throw std::runtime_error("Trying to insert into a full Load/Store queue");
	}
	table.push_back(instr);
}

LoadStoreQueueAdder::LoadStoreQueueAdder(int cycles_ex)
{
	cycleInEx = cycles_ex;
	internalCycle = 0;
	occupied = false;
	instr = nullptr;
}

int LoadStoreQueueAdder::Next()
{
	if (internalCycle == (cycleInEx - 1))
	{
		occupied = false;
		instr = nullptr;
		internalCycle = 0;
		return register_value + offset;
	}
	internalCycle++;
	return -1;
}

void LoadStoreQueueAdder::Dispatch(Instruction* instruction)
{
	instr = instruction;
	occupied = true;
	op_code = instruction->op_code;
}

MemoryUnit::MemoryUnit(int cycles_ex)
{
	internalCycle = 0;
	cycleInEx = cycles_ex;
	occupied = false;
	instr = nullptr;
}

void MemoryUnit::Dispatch(Instruction* instruction)
{
	instr = instruction;
	occupied = true;
}

float MemoryUnit::Next()
{
	if (internalCycle == (cycleInEx - 1))
	{
		occupied = false;
		if (!instr)
			throw std::runtime_error("Passing a NULL ptr to mem unit");

		internalCycle = 0;
		switch (instr->op_code)
		{
		case ld:
		{
			auto ret_val = mainMem.mainMemory[instr->address];
			instr = nullptr;
			return ret_val;
		}
		case sd:
		{
			mainMem.mainMemory[instr->address] = instr->result;
			instr = nullptr;
			return -1;
		}
		}
	}
	internalCycle++;
	return -1;
}
