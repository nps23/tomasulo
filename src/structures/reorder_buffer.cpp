#include <iostream>
#include <deque>

#include "reorder_buffer.h"
#include "../instruction.h"

ReorderBuffer::ReorderBuffer(int robEntries)
{
	entries = robEntries;
}

void ReorderBuffer::insert(Instruction* instr)
{

	table.push_back(instr);
}

bool ReorderBuffer::isFull()
{
	return (entries == table.size());
}

bool ReorderBuffer::isEmpty()
{
	return table.empty();
}

void ReorderBuffer::pop()
{
	table.pop_front();
}

void ReorderBuffer::clear(Instruction* instr)
{
	table.erase(std::remove(table.begin(), table.end(), instr));
}