#pragma once

class ReorderBuffer
{
public:
	int entries;
	int freeEntries;
	ReorderBuffer(int entries);

	void insert();
	bool isFull() const;
};