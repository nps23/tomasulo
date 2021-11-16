#include <string>
#include "dataHoldingStructures.h"
#include "structures/reorder_buffer.h"
#include "structures/reservation_station.h"
#include "structures/functional_units.h"

extern AddReservationStation addRS;
extern FPReservationStation fRs;
extern ReorderBuffer rob2;
extern RAT rat;
extern intReg intRegFile;
extern fpReg fpRegFile;
extern AddFunctinalUnit addFu;
extern FPFunctionalUnit fpFu;

// Non hardware bookeeping variables;
extern std::map<int, Instruction* > idMap;
extern int numCycles;
extern timingDiagram output;

// call this function before storing it in the instruction buffer
void InitializeInstruction(Instruction& instr)
{
	// Read the last instructions value in the ROB. Expect the the ROB to have at least 1 entry.
	int last_id_index = rob2.table.size() - 1;
	instr.instructionId = (rob2.table[last_id_index]->instructionId)++;
	idMap[instr.instructionId] = &instr;
}

bool IssueFetch()
{
	// get instruction pointed to by PC
	// increment or branch
	// InitializeInstruction
	// put into instruction buffer
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
		if (rob2.isFull())
		{
			break;
		}
		rob2.insert(instr);
		instr.rob_busy = true;
		instr.issue_start_cycle = numCycles;
		instr.issue_end_cycle = numCycles;
		instr.state = ex;
		break;
	case ld:
		break;
	case sd:
		break;
	case beq:
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
		rob2.insert(instr);
		instr.issue_start_cycle = numCycles;
		instr.state = ex;
		break;
	}
	case bne:
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
		rob2.insert(instr);
		instr.issue_start_cycle = numCycles;
		instr.state = ex;
		break;
	}
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
			
			addRS.insert(instr);
			// insert the instruction into the ROB
			rob2.insert(instr);

			// update the instructions ROB metadata
			instr.instType = instr.op_code;
			instr.destValue = "r" + std::to_string(instr.dest);
			instr.rob_busy = true;
			instr.issue_start_cycle = numCycles;
			instr.state = ex;
			return true;
	}
	case sub:
	{
		if (rob2.isFull() || addRS.isFull())
		{
			return false;
		}
		// This is not taking the fetch stage into account right now
		instr.issue_start_cycle = numCycles;

		// Insert into RS. Look up the location of the operand.
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

		// insert into the RS 
		addRS.insert(instr);

		// update the instructions ROB metadata
		instr.instType = instr.op_code;
		instr.destValue = "r" + std::to_string(instr.dest);
		instr.rob_busy = true;
		instr.issue_end_cycle = numCycles;
		rob2.insert(instr);
		instr.state = ex;
		return true;
	}
	case add_i:
	{
		if (rob2.isFull() || addRS.isFull())
		{
			return false;
		}
		instr.issue_start_cycle = numCycles;
		std::string left_operand = rat.r_table[instr.r_left_operand];
		//int immediate = instr.immediate;
		if (left_operand[0] == 'r')
		{
			int left_index = (int)left_operand[1] - 48;
			instr.vj = intRegFile.intRegFile[left_index];
			instr.qj = 0;
		}
		else if (left_operand[0] == 'R')
		{
			int index_value = (int)left_operand[1] - 48;
			instr.qj = index_value;
		}
		// insert into the RS
		addRS.insert(instr);
		
		// insert into the ROB
		rob2.insert(instr);;
		// update the instruction's ROB metadats
		instr.instType = instr.op_code;
		instr.destValue = "r" + std::to_string(instr.dest);
		instr.rob_busy = true;
		instr.issue_end_cycle = numCycles;
		instr.state = ex;
		return true;
	}
	default:
		return true;

	}
	return true;
};

bool Ex(Instruction& instruction)
{
	// in the driver function, we call this on every instruction in all Reservation Stations/ROB
	switch (instruction.op_code)
	{
	case beq:
		break;
	case bne:
		break;
	case nop:
		instruction.state = wb;
		instruction.ex_start_cycle = numCycles;
		instruction.ex_end_cycle = numCycles;
		break;
	case add:
		// check to see if an instruction is ready to go to FU, and has an open unit
		// TODO change to for loop to handle multiple function units
		if (instruction.qj == 0 && instruction.qk == 0 && !addFu.occupied)
		{
			// start the ex timer
			instruction.ex_start_cycle = numCycles;
			addFu.dispatch(&instruction);
			return true;
		}
		// instruction is already issued, just cycle the FU
		else if (&instruction == addFu.instr)
		{
			// need to think a little more about the timing/handoff on this one
			int result = addFu.next();
			if (!addFu.occupied)
			{
				instruction.state = wb;
				instruction.result = result;
				instruction.ex_end_cycle = numCycles;
			}
			return true;
		}
	case sub:
		// same as add
		if (instruction.qj == 0 && instruction.qk == 0 && !addFu.occupied)
		{
			instruction.ex_start_cycle = numCycles;
			addFu.dispatch(&instruction);
			return true;
		}

		else if (&instruction == addFu.instr)
		{
			int result = addFu.next();
			if (!addFu.occupied)
			{
				instruction.state = wb;
				instruction.result = result;
				instruction.ex_end_cycle = numCycles;
			}
			return true;
		}
	case add_d:
		if (instruction.qj == 0 && instruction.qk == 0 && !fpFu.occupied)
		{
			instruction.ex_start_cycle = numCycles;
			fpFu.dispatch(&instruction);
			return true;
		}

		else if (&instruction == addFu.instr)
		{
			int result = fpFu.next();
			if (!addFu.occupied)
			{
				instruction.state = wb;
				instruction.result = result;
				instruction.ex_end_cycle = numCycles;
			}
		}
	case sub_d:
		if (instruction.qj == 0 && instruction.qk == 0 && !fpFu.occupied)
		{
			instruction.ex_start_cycle = numCycles;
			fpFu.dispatch(&instruction);
			return true;
		}

		else if (&instruction == addFu.instr)
		{
			int result = fpFu.next();
			if (!addFu.occupied)
			{
				instruction.state = wb;
				instruction.result = result;
				instruction.ex_end_cycle = numCycles;
			}
		}
	case mult_d:
		if (instruction.qj == 0 && instruction.qk == 0 && !fpFu.occupied)
		{
			instruction.ex_start_cycle = numCycles;
			fpFu.dispatch(&instruction);
			return true;
		}

		else if (&instruction == addFu.instr)
		{
			int result = fpFu.next();
			if (!addFu.occupied)
			{
				instruction.state = wb;
				instruction.result = result;
				instruction.ex_end_cycle = numCycles;
			}
		}
	default:
		break;
	}
	return true;
}

bool Mem(Instruction& instr){
	return true;
}
bool WriteBack(Instruction& instr)
{
	switch(instr.op_code)
	{
		case nop:
		{
			instr.state = commit;
			instr.writeback_start_cycle = numCycles;
			instr.writeback_end_cycle = numCycles;
			break;
		}
		default:
			break;
	}
	return true;
}

bool Commit(Instruction& instr)
{
	switch(instr.op_code)
	{
		case nop:
			if((*rob2.table.front()).programLine == instr.programLine){
				instr.state = null;
				rob2.clear();
				instr.commit_start_cycle = numCycles;
				instr.commit_end_cycle = numCycles;
			}
			break;
		default:
			break;
	}
	return true;
}

