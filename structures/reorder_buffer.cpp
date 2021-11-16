#include "reorder_buffer.h"

ReorderBuffer::ReorderBuffer(int entries)
{
	entries = entries;
}

void ReorderBuffer::insert(Instruction& instr)
{

	table.push_back(&instr);
}

bool ReorderBuffer::isFull()
{
	return (unsigned(entries) == table.size());
}

bool ReorderBuffer::isEmpty()
{
	return table.empty();
}

void ReorderBuffer::updateROB()
{

}

void ReorderBuffer::clear()
{
	table.pop_back();
}
