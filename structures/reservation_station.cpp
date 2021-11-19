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
	return table.size() == maxEntries;
}

int AddReservationStation::insert(Instruction* instr)
{
	if (table.size() == maxEntries)
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
	table.erase(std::remove(table.begin(), table.end(), instruction));
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
	if (table.size() == maxEntries)
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
	table.erase(std::remove(table.begin(), table.end(), &instruction));

}