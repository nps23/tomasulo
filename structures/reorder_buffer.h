#pragma once

#include <string>
#include <vector>

#include "../program.h"
#include "../cpu_config.h"


// taken from page 189 of the book
enum ROBStates
{
	rob_ex,
	rob_write_result,
	rob_commit
};

struct ReorderBufferEntry
{
	int index;
	int instType;
	int dest;
	double value;
	bool busy;
	std::string state;
	
	Instruction instruction;
};

class ReorderBuffer
{
private:
	std::vector<ReorderBufferEntry> table;
public:
	ReorderBuffer(const CPUConfig& config)
	{
		int entries = config.rob_entries;
		table.resize(entries);
		for (auto& entry : table)
		{
			std::cout << entry.instType << std::endl;
			entry.instType = 0;
			entry.dest = -1;
			entry.value = 0.0;
			entry.busy = false;
		}
	}

	// return the index of the table entry which the instruction was granted. ff the instruction cannot be issue, return -1 to the caller.
	
	int insert(const Instruction& inst)
	{
		// the way these operands will be buffered into the ROB depend on the instruction type.
		switch (inst.op_code)
		{
		case nop:
			return -1;
		case ld:
			break;
		case sd:
			break;
		case beq:
			break;
		case bne:
			break;
		case add:
			int l_operand = inst.r_right_operand;
			int r_operand = inst.r_left_operand;
			int dest = inst.dest;
			int index = 0;
			for (auto& entry : table)
			{
				if (!entry.busy)
				{
					entry.busy = true;
					entry.instType = add;
					entry.dest = inst.dest;
					entry.state = rob_ex;
					return index;
				}
				index++;
			}
			return -1;
		case add_d:
		{
			int l_operand = inst.f_left_operand;
			int r_operand = inst.f_right_operand;
			int dest = inst.dest;
			int index = 0;
			for (auto& entry : table)
			{
				if (!entry.busy)
				{
					entry.busy = true;
					entry.instType = add_d;
					entry.dest = inst.dest;
					entry.state = rob_ex;
					return index;
				}
				index++;
			}
			return -1;
		}
		case add_i:
			break;
		case sub:
			break;
		case sub_d:
		{
			int l_operand = inst.f_left_operand;
			int r_operand = inst.f_right_operand;
			int dest = inst.dest;
			int index = 0;
			for (auto& entry : table)
			{
				if (entry.busy == false)
				{
					entry.busy = true;
					entry.instType = add_d;
					entry.dest = inst.dest;
					entry.state = rob_ex;
					return index;
				}
				index++;
			}
			return -1;
			break;
		}
		case mult_d:
			break;
		}

		return 0;
	}
	
	bool isFull()
	{
		for (const auto& entry : table)
		{
			if (entry.busy == false)
			{
				return false;
			}
		}
		return true;
	}
};