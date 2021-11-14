#include "functional_units.h"

#include "../instruction.h"

extern int numCycles;

AddFunctinalUnit::AddFunctinalUnit(int cycles_ex)
{
	cycleInEx = cycles_ex;
	internalCycle = 0;
	instr = nullptr;
}

void AddFunctinalUnit::next()
{
	internalCycle++;
	if (internalCycle == cycleInEx)
	{
		instr->ex_end_cycle = numCycles;
	}
}