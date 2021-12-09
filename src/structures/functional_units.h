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

// TODO: The floating point units are pipelined
class FPAddFunctionalUnit
{
public:
	int internalCycle;
	int cycleInEx;
	int op_code;
	double left_operand;
	double right_operand;
	Instruction* instr;
	void dispatch(Instruction* instr);
	double next();
	bool occupied;
	FPAddFunctionalUnit(int cycles_ex);
};

class FPMultFunctionalUnit
{
public:
	int internalCycle;
	int cycleInEx;
	int op_code;
	double left_operand;
	double right_operand;
	Instruction* instr;
	void dispatch(Instruction* instr);
	double next();
	bool occupied;
	FPMultFunctionalUnit(int cycles_ex);
};
