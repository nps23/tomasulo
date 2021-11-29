#include "functional_units.h"

#include "../instruction.h"

extern int numCycles;

AddFunctinalUnit::AddFunctinalUnit(int cycles_ex)
{
	cycleInEx = cycles_ex;
	internalCycle = 0;
	occupied = false;
	instr = nullptr;
}

int AddFunctinalUnit::next()
{
	if (internalCycle == (cycleInEx-1))
	{
		occupied = false;
		instr = nullptr;
		internalCycle = 0;
		switch (op_code)
		{
		case add_i:
			return left_operand + right_operand;
		case add:
			return left_operand + right_operand;
		case sub:
			return left_operand - right_operand;
		case bne:
		{
			if (left_operand == right_operand)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case beq:
		{
			if (left_operand == right_operand)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		default:
			throw "EX logic not found for instruction with op code in integer adder function unit";
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

FPAddFunctionalUnit::FPAddFunctionalUnit(int cycles_ex)
{
	cycleInEx = cycles_ex-1;
	internalCycle = 0;
	instr = nullptr;
	occupied = false;
}

double FPAddFunctionalUnit::next()
{
	if (internalCycle == (cycleInEx-1))
	{
		occupied = false;
		instr = nullptr;
		internalCycle = 0;
		switch (op_code)
		{
		case add_d:
			return left_operand + right_operand;
		case sub_d:
			return left_operand - right_operand;
		case mult_d:
			throw "Trying to pass mult into the functional unit";
		default:
			throw "Tried passing a non FP instruction into the FP unit";
		}
	}
	internalCycle++;
	return -1;
}

void FPAddFunctionalUnit::dispatch(Instruction* instruction)
{
	instr = instruction;
	occupied = true;
	op_code = instruction->op_code;
	left_operand = instruction->vj;
	right_operand = instruction->vk;
}

FPMultFunctionalUnit::FPMultFunctionalUnit(int cycles_ex)
{
	cycleInEx = cycles_ex - 1;
	internalCycle = 0;
	occupied = false;
	instr = nullptr;
}

void FPMultFunctionalUnit::dispatch(Instruction* instruction)
{
	instr = instruction;
	occupied = true;
	op_code = instruction->op_code;
	left_operand = instruction->vj;
	right_operand = instruction->vk;
}

double FPMultFunctionalUnit::next()
{
	if (internalCycle == (cycleInEx-1))
	{
		occupied = false;
		instr = nullptr;
		internalCycle = 0;
		switch (op_code)
		{
		case mult_d:
			return right_operand * left_operand;
		}
	}
	internalCycle++;
	return -1;
}