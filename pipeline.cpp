#include <string>
#include <iostream>
#include "dataHoldingStructures.h"
#include "structures/reorder_buffer.h"
#include "structures/reservation_station.h"
#include "structures/functional_units.h"
#include "structures/central_data_bus.h"
#include "structures/RegisterAliasingTable.h"

extern AddReservationStation addRS;
extern FPReservationStation fRs;
extern ReorderBuffer rob2;
extern RAT rat;
extern intReg intRegFile;
extern fpReg fpRegFile;
extern AddFunctinalUnit addFu;
extern FPAddFunctionalUnit fpFu;
extern FPMultFunctionalUnit fpMulFu;
extern cdb bus;
extern ROM rom;
extern instructionBuffer instBuff;
extern IntRegisterAliasingTable intRat;
extern FPRegisterAliasingTable fpRat;


// Non hardware bookeeping variables;
extern std::map<int, Instruction* > idMap;
extern int numCycles;
extern timingDiagram output;
extern std::vector<Instruction*> outputInstructions;

// call this function before storing an instruction in the instruction buffer
// didn't feel like writing a copy constructor for the struct
Instruction* copyInstruction(const Instruction* source)
{
	Instruction* new_instr = new Instruction;
	new_instr->op_code = source->op_code;
	new_instr->r_right_operand = source->r_right_operand;
	new_instr->r_left_operand = source->r_left_operand;
	new_instr->f_left_operand = source->f_left_operand;
	new_instr->f_right_operand = source->f_right_operand;
	new_instr->offset = source->offset;
	new_instr->immediate = source->immediate;
	new_instr->r_ls_register_operand = source->r_ls_register_operand;
	new_instr->f_ls_register_operand = source->f_ls_register_operand;
	new_instr->dest = source->dest;
	new_instr->result = source->result;
	new_instr->programLine = source->programLine;

	return new_instr;
}

Instruction* InitializeInstruction(Instruction* instr)
{
	// Read the last instructions value in the ROB. Expect the the ROB to have at least 1 entry.
	Instruction* new_instr = copyInstruction(instr);
	// lets try using the last element of the map instead of the ROB.
	
	// we seem to be clearing the ROB before getting to the next stage
	if (rob2.table.size() == 0)
	{
		new_instr->instructionId = 1;
		idMap[1] = new_instr;
		return new_instr;
	}
	int last_id_index = rob2.table.size() - 1;

	//new_instr->instructionId = ((rob2.table[last_id_index])->instructionId) + 1;
	//idMap[new_instr->instructionId] = &instr;

	int new_value = std::prev(idMap.end())->first + 1;
	new_instr->instructionId = new_value;
	idMap[new_value] = new_instr;

	return new_instr;
}


bool IssueFetch(Instruction* instr)
{
	Instruction* fetch_instr = InitializeInstruction(rom.pc);
	fetch_instr->issue_start_cycle = numCycles;
	fetch_instr->state = issue;
	fetch_instr->just_fetched = true;
	instBuff.inst.push_back(fetch_instr);
	rom.pc++;
	//instBuff.inst[instBuff.curInst]->state = issue;
	//instBuff.inst[instBuff.curInst]->just_fetched = true;
	//instBuff.curInst++;
	cout << "entering fetch. Size of inst buffer = " << instBuff.inst.size() << endl;
	
	return true;
}

// At every cycle, call this function with the head of the ROB instruction
// somewhere, we need a new function to create new "Instructions" based on branches 
bool Issue(Instruction* instr)
{
	// Check the type of the instruction
	switch (instr->op_code)
	{
	case nop:
		if (rob2.isFull())
		{
			std::cout << "ROB always full!!" << std::endl;
			break;
		}
		rob2.insert(instr);
		instr->rob_busy = true;
		instr->issue_start_cycle = numCycles;
		instr->issue_end_cycle = numCycles;
		instr->state = ex;
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
		instr->issue_start_cycle = numCycles;
		// Look up the location of the operand.
		std::string left_operand = rat.r_table[instr->r_left_operand];
		std::string right_operand = rat.r_table[instr->r_right_operand];
		if (left_operand[0] == 'r')
		{
			int left_index = (int)left_operand[1] - 48;
			instr->vj = intRegFile.intRegFile[left_index];
			instr->qj = 0;
		}
		// this indicates an ROB lookup instead
		else if (left_operand[0] == 'R')
		{
			int index_value = (int)left_operand[3] - 48;
			instr->qj = index_value;
		}
		if (right_operand[0] == 'r')
		{
			int right_index = (int)right_operand[1] - 48;
			instr->vk = intRegFile.intRegFile[right_index];
			instr->qk = 0;
		}
		else if (right_operand[0] == 'R')
		{
			int index_value = (int)right_operand[3] - 48;
			instr->qj = index_value;
		}
		instr->instType = instr->op_code;
		instr->rob_busy = true;
		rob2.insert(instr);
		instr->issue_start_cycle = numCycles;
		instr->state = ex;
		break;
	}
	case bne:
	{
		if (rob2.isFull() || addRS.isFull())
		{
			break;
		}
		instr->issue_start_cycle = numCycles;
		// Look up the location of the operand.
		std::string left_operand = rat.r_table[instr->r_left_operand];
		std::string right_operand = rat.r_table[instr->r_right_operand];
		if (left_operand[0] == 'r')
		{
			int left_index = (int)left_operand[1] - 48;
			instr->vj = intRegFile.intRegFile[left_index];
			instr->qj = 0;
		}
		// this indicates an ROB lookup instead
		else if (left_operand[0] == 'R')
		{
			int index_value = (int)left_operand[3] - 48;
			instr->qj = index_value;
		}
		if (right_operand[0] == 'r')
		{
			int right_index = (int)right_operand[1] - 48;
			instr->vk = intRegFile.intRegFile[right_index];
			instr->qk = 0;
		}
		else if (right_operand[0] == 'R')
		{
			int index_value = (int)right_operand[3] - 48;
			instr->qj = index_value;
		}
		instr->instType = instr->op_code;
		instr->rob_busy = true;
		rob2.insert(instr);
		instr->issue_start_cycle = numCycles;
		instr->state = ex;
		break;
	}
	case add:
	{
			if (rob2.isFull() || addRS.isFull())
			{
				break;
			}
			
			instBuff.clear(instr);
			auto& l_entry = intRat.table[instr->r_left_operand];
			auto& r_entry = intRat.table[instr->r_right_operand];
			auto& dest = intRat.table[instr->dest];

			if (!l_entry.is_mapped)
			{
				instr->vj = l_entry.value;
				instr->qj = 0;
			}
			else
			{
				instr->qj = l_entry.value;
			}
			if (!r_entry.is_mapped)
			{
				instr->vk = r_entry.value;
				instr->qk = 0;
			}
			else
			{
				instr->qk = r_entry.value;
			}
			// update the ROB, RS, and the RAT
			instr->issue_end_cycle = numCycles;
			addRS.insert(instr);
			rob2.insert(instr);
			dest.is_mapped = true;
			dest.value = instr->instructionId;
			
			// update the instructions ROB metadata
			instr->instType = instr->op_code;
			instr->rob_busy = true;
			// Setting the ex state is handled in the driver function in order to avoid a timing error. 
			instr->issued = true;
			instr->state = ex;
			return true;
	}
	case sub:
	{
		if (rob2.isFull() || addRS.isFull())
		{
			break;
		}

		instBuff.clear(instr);
		auto& l_entry = intRat.table[instr->r_left_operand];
		auto& r_entry = intRat.table[instr->r_right_operand];
		auto& dest = intRat.table[instr->dest];

		if (!l_entry.is_mapped)
		{
			instr->vj = l_entry.value;
			instr->qj = 0;
		}
		else
		{
			instr->qj = l_entry.value;
		}
		if (!r_entry.is_mapped)
		{
			instr->vk = r_entry.value;
			instr->qk = 0;
		}
		else
		{
			instr->qk = r_entry.value;
		}
		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		addRS.insert(instr);
		rob2.insert(instr);
		dest.is_mapped = true;
		dest.value = instr->instructionId;

		instr->instType = instr->op_code;
		instr->rob_busy = true;
		// Setting the ex state is handled in the driver function in order to avoid a timing error. 
		instr->issued = true;
		instr->state = ex;
		return true;
	}
	case add_i:
	{
		if (rob2.isFull() || addRS.isFull())
		{
			return false;
		}
		instr->issue_start_cycle = numCycles;
		std::string left_operand = rat.r_table[instr->r_left_operand];
		//int immediate = instr->immediate;
		if (left_operand[0] == 'r')
		{
			int left_index = (int)left_operand[1] - 48;
			instr->vj = intRegFile.intRegFile[left_index];
			instr->qj = 0;
		}
		else if (left_operand[0] == 'R')
		{
			int index_value = (int)left_operand[1] - 48;
			instr->qj = index_value;
		}
		// insert into the RS
		addRS.insert(instr);
		
		// insert into the ROB
		rob2.insert(instr);;
		// update the instruction's ROB metadats
		instr->instType = instr->op_code;
		instr->rob_busy = true;
		instr->issue_end_cycle = numCycles;
		instr->state = ex;
		return true;
	}
	case mult_d:
	{
		if (rob2.isFull() || fRs.isFull())
		{
			break;
		}

		instBuff.clear(instr);
		auto& l_entry = fpRat.table[instr->f_left_operand];
		auto& r_entry = fpRat.table[instr->f_right_operand];
		auto& dest = fpRat.table[instr->dest];

		if (!l_entry.is_mapped)
		{
			instr->vj = l_entry.value;
			instr->qj = 0;
		}
		else
		{
			instr->qj = l_entry.value;
		}
		if (!r_entry.is_mapped)
		{
			instr->vk = r_entry.value;
			instr->qk = 0;
		}
		else
		{
			instr->qk = r_entry.value;
		}
		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		fRs.insert(instr);
		rob2.insert(instr);
		dest.is_mapped = true;
		dest.value = instr->instructionId;

		// update the instructions ROB metadata
		instr->instType = instr->op_code;
		instr->rob_busy = true;
		// Setting the ex state is handled in the driver function in order to avoid a timing error. 
		instr->issued = true;
		instr->state = ex;
		return true;
	}
	default:
		return true;

	}
	return true;
};

// probably don't want this to return bool
bool Ex(Instruction* instruction)
{
	// in the driver function, we call this on every instruction in all Reservation Stations/ROB
	switch (instruction->op_code)
	{
	case beq:
		break;
	case bne:
		break;
	case nop:
		instruction->state = wb;
		instruction->ex_start_cycle = numCycles;
		instruction->ex_end_cycle = numCycles;
		break;
	case add:
		// TODO change to for loop to handle multiple function units
		if (instruction->qj == 0 && instruction->qk == 0 && !addFu.occupied)
		{
			// start the ex timer
			instruction->ex_start_cycle = numCycles;
			addFu.dispatch(instruction);
			addRS.clear(instruction);
			return true;
		}
		// instruction is already issued, just cycle the FU
		else if (instruction == addFu.instr)
		{
			int result = addFu.next();
			if (!addFu.occupied)
			{
				instruction->state = wb;
				instruction->result = result;
				instruction->ex_end_cycle = numCycles;
			}
			return true;
		}
		break;
	case sub:
		// same as add
		if (instruction->qj == 0 && instruction->qk == 0 && !addFu.occupied)
		{
			instruction->ex_start_cycle = numCycles;
			addFu.dispatch(instruction);
			addRS.clear(instruction);
			return true;
		}

		else if (instruction == addFu.instr)
		{
			int result = addFu.next();
			if (!addFu.occupied)
			{
				instruction->state = wb;
				instruction->result = result;
				instruction->ex_end_cycle = numCycles;
			}
			return true;
		}
	case add_d:
		if (instruction->qj == 0 && instruction->qk == 0 && !fpFu.occupied)
		{
			instruction->ex_start_cycle = numCycles;
			fpFu.dispatch(instruction);
			return true;
		}

		else if (instruction == addFu.instr)
		{
			double result = fpFu.next();
			if (!addFu.occupied)
			{
				instruction->state = wb;
				instruction->result = result;
				instruction->ex_end_cycle = numCycles;
			}
		}
	case sub_d:
		if (instruction->qj == 0 && instruction->qk == 0 && !fpFu.occupied)
		{
			instruction->ex_start_cycle = numCycles;
			fpFu.dispatch(instruction);
			return true;
		}

		else if (instruction == addFu.instr)
		{
			double result = fpFu.next();
			if (!addFu.occupied)
			{
				instruction->state = wb;
				instruction->result = result;
				instruction->ex_end_cycle = numCycles;
			}
		}
	case mult_d:
		if (instruction->qj == 0 && instruction->qk == 0 && !fpMulFu.occupied)
		{
			instruction->ex_start_cycle = numCycles;
			fpMulFu.dispatch(instruction);
			return true;
		}

		else if (instruction == fpMulFu.instr)
		{
			double result = fpMulFu.next();
			if (!fpMulFu.occupied)
			{
				instruction->state = wb;
				instruction->result = result;
				instruction->ex_end_cycle = numCycles;
			}
		}
	case ld:
		break;
	case sd:
		break;
	default:
		break;
	}
	return true;
}

bool Mem(Instruction* instr){
	return true;
}
bool WriteBack(Instruction* instr)
{
	switch(instr->op_code)
	{
		case nop:
		{
			instr->state = commit;
			instr->writeback_start_cycle = numCycles;
			instr->writeback_end_cycle = numCycles;
			break;
		}
		case ld:
			break;
		case sd:
			break;
		case beq:
			break;
		case bne:
			break;
		case fin:
			break;

		default:  // should work for add, add_d, add_i sub, sub_d, mult_d
		{
			if (instr->writeback_begin)
			{
				instr->writeback_begin = false;
				instr->writeback_start_cycle = numCycles;
			}
			if (!bus.occupied)
			{
				if (!bus.isEmpty())
				{
					bus.clear(instr);
				}
				bus.occupied = true;
				// TODO account for mappping logic
				for (auto& instruction : addRS.table)
				{
					if (instruction->qj == instr->instructionId)
					{
						instruction->qj = 0;
						instruction->vj = instr->result;
					}
					if (instruction->qk == instruction->instructionId)
					{
						instruction->qk = 0;
						instruction->vk = instr->result;
					}
				}
				for (auto& instruction : fRs.table)
				{
					if (instruction->qj == instr->instructionId)
					{
						instruction->qj = 0;
						instruction->vj = instr->result;
					}
					if (instruction->qk == instruction->instructionId)
					{
						instruction->qk = 0;
						instruction->vk = instr->result;
					}
				}


				instr->writeback_end_cycle = numCycles;
				instr->state = commit;
				instr->commit_start_cycle = numCycles + 1;
			}
			else
			{
				bus.insert(instr);
			}
		}
	}
	return true;
}

// this should be called once per cycle on the head of the ROB
bool Commit(Instruction* instr)
{
	switch (instr->op_code)
	{
	case nop:
		if (((*rob2.table.front()).state == commit) && ((*rob2.table.front()).programLine == instr->programLine))
		{
			instr->state = null;
			rob2.clear();
			instr->commit_start_cycle = numCycles;
			instr->commit_end_cycle = numCycles;
			outputInstructions.push_back(instr);
		}
		break;
	case add:
	{
		if (instr->commit_begin)
		{
			instr->commit_begin = false;
			instr->commit_start_cycle = numCycles;
		}

		if (instr == rob2.table[0] && !rob2.hasCommited)
		{
			rob2.hasCommited = true;
			int result = instr->result;
			int index = instr->dest;
			intRegFile.intRegFile[index] = result;
			intRat.table[index].is_mapped = false;
			intRat.table[index].value = result;
			instr->commit_end_cycle = numCycles;
			rob2.clear();
			outputInstructions.push_back(instr);
		}
		break;
	}
	case sub:
	{
		if (instr->commit_begin)
		{
			instr->commit_begin = false;
			instr->commit_start_cycle = numCycles;
		}

		if (instr == rob2.table[0] && !rob2.hasCommited)
		{
			rob2.hasCommited = true;
			int result = instr->result;
			int index = instr->dest;
			intRegFile.intRegFile[index] = result;
			intRat.table[index].is_mapped = false;
			intRat.table[index].value = result;
			instr->commit_end_cycle = numCycles;
			rob2.clear();
			outputInstructions.push_back(instr);
		}
		break;
	}
	case mult_d:
	{
		if (instr->commit_begin)
		{
			instr->commit_begin = false;
			instr->commit_start_cycle = numCycles;
		}

		if (instr == rob2.table[0] && !rob2.hasCommited)
		{
			rob2.hasCommited = true;
			int result = instr->result;
			int index = instr->dest;
			intRegFile.intRegFile[index] = result;
			fpRat.table[index].is_mapped = false;
			fpRat.table[index].value = result;
			instr->commit_end_cycle = numCycles;
			rob2.clear();
			outputInstructions.push_back(instr);
		}
		break;
	}
	default:
	{
		throw "NO COMMIT IMPLEMENTED FOR INSTRUCTION YET";
	}
	}
}


