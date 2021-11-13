#include "reorder_buffer.h"

ReorderBuffer::ReorderBuffer(int entries)
{
	entries = entries;
	freeEntries = entries;
}

void ReorderBuffer::insert()
{
	freeEntries--;
}

bool ReorderBuffer::isFull() const
{
	return (freeEntries == 0);
}