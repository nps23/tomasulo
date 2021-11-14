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


// return true if the instruction was able to be issued
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

		if (!rob2.isFull() && !addRS.isFull())
		{
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
			instr.issue_start_cycle = numCycles;
			instr.robEntry = rob2.insert(instr);
			if (numCycles == instr.issue_end_cycle)
			{

			}
		}
		break;
	default:
		break;

	}
	return true;
};

void Ex()
{
	std::cout << "Hello" << std::endl;
}

void WriteBack()
{

}

void Commit()
{

}