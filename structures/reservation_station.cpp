#include "reservation_station.h"

AddReservationStation::AddReservationStation(int entries)
{
	maxEntries= entries;
	freeEntries = entries;
	table.resize(entries);
}

bool AddReservationStation::isFull()
{
	return (freeEntries == 0);
}

int AddReservationStation::insert(const Instruction& instr)
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
void AddReservationStation::updateRS()
{
	for (auto it = table.begin(); it != table.end();)
	{
		if ((*it).state != issue)
		{
			it = table.erase(it);
			freeEntries++;
		}
		else
		{
			++it;
		}
	}
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

int FPReservationStation::insert(const Instruction& instr)
{
	if (freeEntries == 0)
	{
		throw "Trying to insert into a full FP Reservations station";
	}
	table.push_back(instr);
	freeEntries--;
	int index = table.size() - 1;
	return index;
}

void FPReservationStation::updateRS()
{
	for (auto it = table.begin(); it != table.end();)
	{
		if (!(*it).rs_busy)
		{
			it = table.erase(it);
			freeEntries++;
		}
		else
		{
			++it;
		}
	}
}