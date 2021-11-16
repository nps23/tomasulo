#include "reorder_buffer.h"

ReorderBuffer::ReorderBuffer(int entries)
{
	entries = entries;
	freeEntries = entries;
}

int ReorderBuffer::insert(const Instruction& instr)
{
	
	table.push(instr);
	int rob_index = entries - freeEntries;
	freeEntries--;
	return rob_index;
}

bool ReorderBuffer::isFull() const
{
	return (freeEntries == 0);
}

void ReorderBuffer::updateROB()
{

}