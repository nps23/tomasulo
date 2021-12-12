#pragma once

#include <vector>

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
	int cycleInEx;
	void dispatch(Instruction* instr);
	double next(Instruction* input);
	FPAddFunctionalUnit(int cycles_ex);
	bool instruction_dispatched_on_current_cycle;
	std::vector<Instruction*> fp_instructions;
	void Clear(Instruction* instr);
};

class FPMultFunctionalUnit
{
public:
	int cycleInEx;
	void dispatch(Instruction* instr);
	double next(Instruction* input);
	FPMultFunctionalUnit(int cycles_ex);
	bool instruction_dispatched_on_current_cycle;
	std::vector<Instruction*> fp_instructions;
	void Clear(Instruction* instr);
};
