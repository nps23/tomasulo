#pragma once

#include "../instruction.h"

class AddFunctinalUnit
{
public:
	int internalCycle;
	int cycleInEx;
	int op_code;
	int left_operand;
	int right_operand;
	Instruction* instr;
	void dispatch(Instruction* instruction);
	int next();
	bool occupied;
	AddFunctinalUnit(int cycles_ex);
};

class FPFunctionalUnit
{
public:
	int internalCycle;
	int cycleInEx;
	int op_code;
	int left_operand;
	int right_operand;
	Instruction* instr;
	void dispatch(Instruction* instr);
	double next();
	bool occupied;
	FPFunctionalUnit(int cycles_ex);
};

