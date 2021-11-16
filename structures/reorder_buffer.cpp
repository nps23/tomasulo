#include "reorder_buffer.h"

ReorderBuffer::ReorderBuffer(int entries)
{
	entries = entries;
}

void ReorderBuffer::insert(Instruction& instr)
{

	table.push_back(&instr);
}

bool ReorderBuffer::isFull() const
{
	return (entries == table.size());
}

bool ReorderBuffer::isEmpty() const
{
	return table.empty();
}

void ReorderBuffer::updateROB()
{

}

void ReorderBuffer::clear(const Instruction& instr)
{
	table.pop_back();
}