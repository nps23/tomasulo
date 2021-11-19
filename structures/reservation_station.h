#pragma once

#include <queue>
#include <vector>

#include "../instruction.h"

class FPReservationStation
{
public:
	int maxEntries;
	int freeEntries;
	std::vector<Instruction*> table;
	bool isFull();
	int insert(Instruction* instr);
	void clear(Instruction* instruction);
	FPReservationStation(int entries);
};

class AddReservationStation
{
public:
	int maxEntries;
	int freeEntries;
	std::vector<Instruction*> table;
	bool isFull();
	int insert(Instruction* instr);
	void clear(Instruction* instruction);
	AddReservationStation(int entries);
};
