#include <iostream>

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
			throw std::runtime_error("EX logic not found for instruction with op code in integer adder function unit");
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
	cycleInEx = cycles_ex - 1; // starting from a zero index thing
	instruction_dispatched_on_current_cycle = false;
	fp_instructions.resize(0);
}

void FPAddFunctionalUnit::Clear(Instruction* instr)
{
	fp_instructions.erase(std::remove(fp_instructions.begin(), fp_instructions.end(), instr));
}

double FPAddFunctionalUnit::next(Instruction* input)
{
	if (input->internal_fp_cycle == cycleInEx)
	{
		input->fp_unit_complete = true;
		input->occupying_fp_unit = false;
		Clear(input);
		switch (input->op_code)
		{
		case add_d:
			return input->vj + input->vj;
		case sub_d:
			input->vj - input->vk;
		default:
			throw std::runtime_error("Instuction should not be going into the FP add function unit!");
		}
	}
	input->internal_fp_cycle++;
	return -1;
}

void FPAddFunctionalUnit::dispatch(Instruction* instruction)
{
	instruction->occupying_fp_unit = true;
	instruction_dispatched_on_current_cycle = true;
	fp_instructions.push_back(instruction);
}

FPMultFunctionalUnit::FPMultFunctionalUnit(int cycles_ex)
{
	cycleInEx = cycles_ex - 1;
	instruction_dispatched_on_current_cycle = false;
	fp_instructions.resize(0);
}

void FPMultFunctionalUnit::dispatch(Instruction* instruction)
{
	instruction->occupying_fp_unit = true;
	instruction_dispatched_on_current_cycle = true;
	fp_instructions.push_back(instruction);
}

void FPMultFunctionalUnit::Clear(Instruction* instr)
{
	fp_instructions.erase(std::remove(fp_instructions.begin(), fp_instructions.end(), instr));
}

double FPMultFunctionalUnit::next(Instruction* input)
{
	if (input->internal_fp_cycle == cycleInEx)
	{
		input->fp_unit_complete = true;
		input->occupying_fp_unit = false;
		Clear(input);
		switch (input->op_code)
		{
		case mult_d:
			return input->vj + input->vj;
		default:
			throw std::runtime_error("Instuction should not be going into the FP mult function unit!");
		}
	}
	input->internal_fp_cycle++;
	return -1;
}