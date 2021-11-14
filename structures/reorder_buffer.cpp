#include "reorder_buffer.h"

ReorderBuffer::ReorderBuffer(int entries)
{
	entries = entries;
	freeEntries = entries;
}

int ReorderBuffer::insert(const Instruction& instr)
{
	freeEntries--;
	table.push(instr);
	int rob_index = entries - freeEntries;
	return rob_index;
}

bool ReorderBuffer::isFull() const
{
	return (freeEntries == 0);
}

void ReorderBuffer::updateROB()
{
	// check each instruction sequentially. If an instruction
}