#include "instruction.h"
#include "dataHoldingStructures.h"
#include "structures/reorder_buffer.h"
#include "structures/reservation_station.h"

extern AddReservationStation addRS;
extern FPReservationStation fRs;
extern ReorderBuffer rob2;
extern RAT rat;

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
			// Look up the locaiton of the operand.
			std::string left_operand = rat.r_table[instr.r_left_operand];
			std::string right_operand = rat.r_table[instr.r_right_operand];
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