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
	std::cout << "The number of entries = " << entries << " The size of the table is: " << table.size() << std::endl;
	return (entries == table.size());
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
	table.pop_front();
}
