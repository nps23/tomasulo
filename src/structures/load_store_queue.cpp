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
	register_value = -1;				// The value the (integer) holds that 
}

int LoadStoreQueueAdder::Next()
{
	if (internalCycle == cycleInEx)
	{
		occupied = false;
		instr = nullptr;
		internalCycle = 0;
		int ret_val = register_value + offset;
		register_value = -1;
		return ret_val;
	}
	internalCycle++;
	return -1;
}

void LoadStoreQueueAdder::Dispatch(Instruction* instruction)
{
	instr = instruction;
	offset = instr->offset;
	register_value = instr->vj;
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
			mainMem.mainMemory[instr->address] = instr->vk;
			instr = nullptr;
			return -1;
		}
		}
	}
	internalCycle++;
	return -1;
}
