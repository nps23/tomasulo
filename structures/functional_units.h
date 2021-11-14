#pragma once

#include "../instruction.h"

class AddFunctinalUnit
{
	int internalCycle;
	int cycleInEx;
	Instruction* instr;
	void dispatch(Instruction& instr);
	void next();
	bool occupied();
	AddFunctinalUnit(int cycles_ex);
};

class FPFunctionalUnit
{
	int internalCycle;
	Instruction* instr;
	void next();
};

