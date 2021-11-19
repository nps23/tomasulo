#include "functional_units.h"

#include "../instruction.h"

extern int numCycles;

AddFunctinalUnit::AddFunctinalUnit(int cycles_ex)
{
	cycleInEx = cycles_ex - 1;
	internalCycle = 0;
	occupied = false;
	instr = nullptr;
}

int AddFunctinalUnit::next()
{
	if (internalCycle == cycleInEx)
	{
		occupied = false;
		instr = nullptr;
		internalCycle = 0;
		switch (op_code)
		{
		case add:
			return left_operand + right_operand;
		case sub:
			return left_operand - right_operand;
		}
	}
	internalCycle++;
	return -1;
}

void AddFunctinalUnit::dispatch(Instruction* instruction)
{
	instr = instruction;
	occupied = true;
	op_code = instruction->op_code;
	left_operand = (int)instruction->vj;
	right_operand = (int)instruction->vk;
}

FPFunctionalUnit::FPFunctionalUnit(int cycles_ex)
{
	cycleInEx = cycles_ex;
	internalCycle = 0;
	instr = nullptr;
	occupied = false;
}

double FPFunctionalUnit::next()
{
	if (internalCycle == cycleInEx)
	{
		occupied = false;
		instr = nullptr;
		internalCycle = 0;
		switch (op_code)
		{
		case add_d:
			return left_operand + right_operand;
		case sub_d:
			return right_operand - left_operand;
		case mult_d:
			return right_operand * left_operand;
		default:
			throw "Tried passing a non FP instruction into the FP unit";
		}
	}
	internalCycle++;
	return -1;
}

void FPFunctionalUnit::dispatch(Instruction* instruction)
{
	instr = instruction;
	occupied = true;
	op_code = instruction->op_code;
	left_operand = instruction->vj;
	right_operand = instruction->vk;
}