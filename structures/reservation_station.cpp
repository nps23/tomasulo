#include <algorithm>

#include "reservation_station.h"

AddReservationStation::AddReservationStation(int entries)
{
	table.resize(0);
	maxEntries= entries;
	freeEntries = entries;
}

bool AddReservationStation::isFull()
{
	return (freeEntries == 0);
}

int AddReservationStation::insert(Instruction* instr)
{
	if (freeEntries == 0)
	{
		throw "Trying to insert into a full reservation station";
	}
	table.push_back(instr);
	freeEntries--;
	int index = table.size() - 1;
	return index;
}

// should be called at the end of each pipeline to account for the transition instructions
void AddReservationStation::clear(Instruction* instruction)
{
	if (instruction->qj != 0 || instruction->qk != 0 || table.size() == 0)
	{
		throw "Error! Trying to remove an element from the Add RS before operands are ready";
	}
	
	table.erase(std::remove(table.begin(), table.end(), instruction));
	/*
	for (auto it = table.begin(); it != table.end();)
	{
		if ((*it)->state != issue)
		{
			it = table.erase(it);
			freeEntries++;
		}
		else
		{
			++it;
		}
	}
	*/
}

FPReservationStation::FPReservationStation(int entries)
{
	maxEntries = entries;
	freeEntries = entries;
}

bool FPReservationStation::isFull()
{
	return (freeEntries == 0);
}

int FPReservationStation::insert(Instruction& instr)
{
	if (freeEntries == 0)
	{
		throw "Trying to insert into a full FP Reservations station";
	}
	table.push_back(&instr);
	freeEntries--;
	int index = table.size() - 1;
	return index;
}

void FPReservationStation::clear(Instruction& instruction)
{
	if (instruction.qj != 0 || instruction.qk != 0 || table.size() == 0)
	{
		throw "Error! Trying to remove an element from the Add RS before operands are ready";
	}

	table.erase(std::remove(table.begin(), table.end(), &instruction));

	/*
	for (auto it = table.begin(); it != table.end();)
	{
		if (!(*it)->rs_busy)
		{
			it = table.erase(it);
			freeEntries++;
		}
		else
		{
			++it;
		}
	}
	*/
}