#pragma once

#include <queue>
#include <vector>

#include "../instruction.h"

class FPReservationStation
{
public:
	int maxEntries;
	int freeEntries;
	std::vector<Instruction> table;
	bool isFull();
	int insert(const Instruction& instr);
	void updateRS();
	FPReservationStation(int entries);
};

class AddReservationStation
{
public:
	int maxEntries;
	int freeEntries;
	std::vector<Instruction> table;
	bool isFull();
	int insert(const Instruction& instr);
	void updateRS();
	AddReservationStation(int entries);
};
