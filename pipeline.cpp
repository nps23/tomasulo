#include <string>

#include "instruction.h"
#include "dataHoldingStructures.h"
#include "structures/reorder_buffer.h"
#include "structures/reservation_station.h"

extern AddReservationStation addRS;
extern FPReservationStation fRs;
extern ReorderBuffer rob2;
extern RAT rat;
extern intReg intRegFile;
extern fpReg fpRegFile;
extern int numCycles;


bool IssueFetch()
{
	return true;
	//
}

// At every cycle, call this function with the head of the ROB instruction
// somewhere, we need a new function to create new "Instructions" based on branches 
bool Issue(Instruction& instr)
{
	// Check the type of the instruction
	switch (instr.op_code)
	{
	case nop:
		break;
	case ld:
		break;
	case sd:
		break;
	case beq:
		break;
	case bne:
		break;
	case add:
	{
			if (rob2.isFull() || addRS.isFull())
			{
				break;
			}
			instr.issue_start_cycle = numCycles;
			// Look up the location of the operand.
			std::string left_operand = rat.r_table[instr.r_left_operand];
			std::string right_operand = rat.r_table[instr.r_right_operand];
			if (left_operand[0] == 'r')
			{
				int left_index = (int)left_operand[1] - 48;
				instr.vj = intRegFile.intRegFile[left_index];
				instr.qj = 0;
			}
			// this indicates an ROB lookup instead
			else if (left_operand[0] == 'R')
			{
				int index_value = (int)left_operand[3] - 48;
				instr.qj = index_value;
			}
			if (right_operand[0] == 'r')
			{
				int right_index = (int)right_operand[1] - 48;
				instr.vk = intRegFile.intRegFile[right_index];
				instr.qk = 0;
			}
			else if (right_operand[0] == 'R')
			{
				int index_value = (int)right_operand[3] - 48;
				instr.qj = index_value;
			}
			instr.instType = instr.op_code;
			instr.destValue = "r" + std::to_string(instr.dest);
			instr.rob_busy = true;
			instr.robEntry = rob2.insert(instr);
			instr.issue_start_cycle = numCycles;
			instr.state = ex;
	}
		break;
	case sub:
		{
			if (rob2.isFull() || addRS.isFull())
			{
				return false;
			}
			// This is not taking the fetch stage into account right now
			instr.issue_start_cycle = numCycles;

			// Look up the location of the operand.
			std::string left_operand = rat.r_table[instr.r_left_operand];
			std::string right_operand = rat.r_table[instr.r_right_operand];
			if (left_operand[0] == 'r')
			{
				int left_index = (int)left_operand[1] - 48;
				instr.vj = intRegFile.intRegFile[left_index];
				instr.qj = 0;
			}
			// this indicates an ROB lookup instead
			else if (left_operand[0] == 'R')
			{
				int index_value = (int)left_operand[3] - 48;
				instr.qj = index_value;
			}
			if (right_operand[0] == 'r')
			{
				int right_index = (int)right_operand[1] - 48;
				instr.vk = intRegFile.intRegFile[right_index];
				instr.qk = 0;
			}
			else if (right_operand[0] == 'R')
			{
				int index_value = (int)right_operand[3] - 48;
				instr.qj = index_value;
			}
			instr.instType = instr.op_code;
			instr.destValue = "r" + std::to_string(instr.dest);
			instr.rob_busy = true;
			instr.issue_end_cycle= numCycles;
			instr.robEntry = rob2.insert(instr);
			instr.state = ex;
		}
		return true;
	default:
		return true;

	}
};

bool Ex(const Instruction& instr)
{
	// in the driver function, we call this on every instruction in all Reservation Stations
	switch (instr.op_code)
	{
		// if cycles ex == 0, then set the cycle counter
	case add:
		// TODO change to for loop to handle multiple function units
		if (instr.qj == 0 && instr.qk == 0 && !fpUnit.occupied)
		{

		}
		break;
	}

}

void WriteBack()
{

}

void Commit()
{

}