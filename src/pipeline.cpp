#include <string>
#include <iostream>
#include "dataHoldingStructures.h"
#include "structures/reorder_buffer.h"
#include "structures/reservation_station.h"
#include "structures/functional_units.h"
#include "structures/central_data_bus.h"
#include "structures/RegisterAliasingTable.h"
#include "structures/BranchPredictor.h"

extern AddReservationStation addRS;
extern FPReservationStation fRS;
extern ReorderBuffer rob;
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
extern BranchPredictor branchPredictor;

// Non hardware bookeeping variables;
extern std::map<int, Instruction* > idMap;
extern int numCycles;
extern timingDiagram output;
extern std::vector<Instruction*> outputInstructions;

// Stall controllers
extern bool stall_fetch;

// call this function before storing an instruction in the instruction buffer
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
	new_instr->btb_index = source->btb_index;
	new_instr->source_instruction = rom.pc;

	return new_instr;
}

Instruction* InitializeInstruction(Instruction* instr)
{
	// Read the last instructions value in the ROB. Expect the the ROB to have at least 1 entry.
	Instruction* new_instr = copyInstruction(instr);
	// lets try using the last element of the map instead of the ROB.
	
	// we seem to be clearing the ROB before getting to the next stage
	if (rob.table.size() == 0 && numCycles == 0)
	{
		new_instr->instructionId = 1;
		idMap[1] = new_instr;
		return new_instr;
	}

	int new_value = std::prev(idMap.end())->first + 1;
	new_instr->instructionId = new_value;
	idMap[new_value] = new_instr;

	return new_instr;
}

// Reset the program counter to the previous value if we branched on a non branch instruction
void ResetPC(Instruction* instr)
{
	// Set the program counter to the next instruction prior to the bad branch
	rom.pc = (instr->source_instruction) + 1;
	instr->triggered_branch = false;
	// clear everything in the instruction buffer accept for the head
	while (instBuff.inst.size() > 1)
	{
		instBuff.inst.pop_back();
	}
}

// Utility function to handle squashing all bad instrucitons after a mispredict
void MispredictSquash(Instruction* instr)
{
	if (!instr->mispredict)
	{ 
		std::cout << "No mispredict! Are you sure you should be calling this fucntion?" << std::endl;
		return;
	}
	
	stall_fetch = true;
	// clear instruction buffer
	while (instBuff.inst.size() > 0)
	{
		instBuff.inst.pop_back();
	}
	// set the PC to the correct locaiton
	if (instr->branch_false_positive)
		rom.pc = instr->source_instruction + 1;
	else if (instr->branch_false_negative)
		rom.pc = instr->realized_instruction_target;
	// clear RS of incorrect instrutions past the branch
	for (auto& entry : addRS.table) 
	{
		if (entry->instructionId > instr->instructionId)
		{
			addRS.clear(instr);
			entry->state = stop;
		}
	}
	for (auto& entry : fRS.table) 
	{
		if (entry->instructionId > instr->instructionId) 
		{
			fRS.clear(instr);
			entry->state = stop;
		}
	}
	// Clear ROB entries
	for (auto& entry : rob.table)
	{
		if (entry->instructionId > instr->instructionId)
		{
			rob.clear(instr);
		}
	}
}

// PIPELINE FUNCTIONS
// TODO: Figure out if this is pipeliend with IssueDecode
bool IssueFetch(Instruction* instr)
{
	Instruction* fetch_instr = InitializeInstruction(rom.pc);
	fetch_instr->issue_start_cycle = numCycles;
	fetch_instr->state = issue;
	fetch_instr->just_fetched = true;
	instBuff.inst.push_back(fetch_instr);
	// Lookup the BTB index, predict a branch
	int btb_index = (rom.pc->btb_index) % 8;
	bool branch_taken = branchPredictor.table[btb_index].first;

	if (branch_taken)
	{
		Instruction* target_branch = branchPredictor.table[btb_index].second;
		rom.pc = target_branch;
		instr->triggered_branch = true;
		instr->source_instruction = rom.pc;
	}
	else
	{
		rom.pc++;
	}

	return true;
}

bool IssueDecode(Instruction* instr)
{
	// Check the type of the instruction
	switch (instr->op_code)
	{
	case nop:
	{
		if (rob.isFull())
		{
			break;
		}
		rob.insert(instr);
		instr->rob_busy = true;
		instr->issue_start_cycle = numCycles;
		instr->issue_end_cycle = numCycles;
		instr->state = ex;
	}
		break;
	case ld:
	{
		if (instr->triggered_branch)
		{
			ResetPC(instr);
		}
	}
		break;
	case sd:
	{
		if (instr->triggered_branch)
		{
			ResetPC(instr);
		}
	}
		break;
	case beq:
	{
		// compute realized target of the branch instruction
		instr->realized_instruction_target = instr->source_instruction + 1 + instr->offset;
		// if we branched to the wrong address, reset right away
		if (instr->btb_target_instruction != instr->realized_instruction_target && instr->triggered_branch)
		{
			ResetPC(instr);
			stall_fetch = true;
		}
		
		if (rob.isFull() || addRS.isFull())
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
			instr->qj = l_entry.map_value;
		}
		if (!r_entry.is_mapped)
		{
			instr->vk = r_entry.value;
			instr->qk = 0;
		}
		else
		{
			instr->qk = r_entry.map_value;
		}
		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		addRS.insert(instr);
		rob.insert(instr);
		dest.is_mapped = true;
		dest.map_value = instr->instructionId;

		// update the instructions ROB metadata
		instr->instType = instr->op_code;
		instr->rob_busy = true;
		// Setting the ex state is handled in the driver function in order to avoid a timing error. 
		instr->issued = true;
		instr->state = ex;
		return true;
	}
	case bne:
	{
		// compute realized target of the branch instruction
		instr->realized_instruction_target = instr->source_instruction + 1 + instr->offset;
		// if we branched to the wrong address, reset right away
		if (instr->btb_target_instruction != instr->realized_instruction_target && instr->triggered_branch)
		{
			ResetPC(instr);
			stall_fetch = true;
		}

		if (rob.isFull() || addRS.isFull())
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
			instr->qj = l_entry.map_value;
		}
		if (!r_entry.is_mapped)
		{
			instr->vk = r_entry.value;
			instr->qk = 0;
		}
		else
		{
			instr->qk = r_entry.map_value;
		}
		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		addRS.insert(instr);
		rob.insert(instr);
		dest.is_mapped = true;
		dest.map_value = instr->instructionId;

		// update the instructions ROB metadata
		instr->instType = instr->op_code;
		instr->rob_busy = true;
		// Setting the ex state is handled in the driver function in order to avoid a timing error. 
		instr->issued = true;
		instr->state = ex;
		return true;
	}
	case add:
	{		
		if (instr->triggered_branch)
		{
			ResetPC(instr);
		}
		if (rob.isFull() || addRS.isFull())
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
			instr->qj = l_entry.map_value;
		}
		if (!r_entry.is_mapped)
		{
			instr->vk = r_entry.value;
			instr->qk = 0;
		}
		else
		{
			instr->qk = r_entry.map_value;
		}
		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		addRS.insert(instr);
		rob.insert(instr);
		dest.is_mapped = true;
		dest.map_value = instr->instructionId;

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
		if (instr->triggered_branch)
		{
			ResetPC(instr);
		}
		if (rob.isFull() || addRS.isFull())
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
			instr->qj = l_entry.map_value;
		}
		if (!r_entry.is_mapped)
		{
			instr->vk = r_entry.value;
			instr->qk = 0;
		}
		else
		{
			instr->qk = r_entry.map_value;
		}
		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		addRS.insert(instr);
		rob.insert(instr);
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
		if (instr->triggered_branch)
		{
			ResetPC(instr);
		}
		if (rob.isFull() || addRS.isFull())
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
			instr->qj = l_entry.map_value;
		}

		// The immediate value will always be given, so fill vk directly.
		instr->vk = instr->immediate;

		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		addRS.insert(instr);
		rob.insert(instr);
		dest.is_mapped = true;
		dest.map_value = instr->instructionId;

		// update the instructions ROB metadata
		instr->instType = instr->op_code;
		instr->rob_busy = true;
		// Setting the ex state is handled in the driver function in order to avoid a timing error. 
		instr->issued = true;
		return true;
	}
	case mult_d:
	{
		if (instr->triggered_branch)
		{
			ResetPC(instr);
		}
		if (rob.isFull() || fRS.isFull())
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
			instr->qj = l_entry.map_value;
		}
		if (!r_entry.is_mapped)
		{
			instr->vk = r_entry.value;
			instr->qk = 0;
		}
		else
		{
			instr->qk = r_entry.map_value;
		}
		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		fRS.insert(instr);
		rob.insert(instr);
		dest.is_mapped = true;
		dest.map_value = instr->instructionId;

		// update the instructions ROB metadata
		instr->instType = instr->op_code;
		instr->rob_busy = true;
		// Setting the ex state is handled in the driver function in order to avoid a timing error. 
		instr->issued = true;
		instr->state = ex;
		return true;
	}
	case add_d:
	{
		if (instr->triggered_branch)
		{
			ResetPC(instr);
		}
		if (rob.isFull() || fRS.isFull())
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
			instr->qj = l_entry.map_value;
		}
		if (!r_entry.is_mapped)
		{
			instr->vk = r_entry.value;
			instr->qk = 0;
		}
		else
		{
			instr->qk = r_entry.map_value;
		}
		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		fRS.insert(instr);
		rob.insert(instr);
		dest.is_mapped = true;
		dest.map_value = instr->instructionId;

		// update the instructions ROB metadata
		instr->instType = instr->op_code;
		instr->rob_busy = true;
		// Setting the ex state is handled in the driver function in order to avoid a timing error. 
		instr->issued = true;
		instr->state = ex;
		return true;
	}
	case sub_d:
	{
		if (instr->triggered_branch)
		{
			ResetPC(instr);
		}
		if (rob.isFull() || fRS.isFull())
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
			instr->qj = l_entry.map_value;
		}
		if (!r_entry.is_mapped)
		{
			instr->vk = r_entry.value;
			instr->qk = 0;
		}
		else
		{
			instr->qk = r_entry.map_value;
		}
		// update the ROB, RS, and the RAT
		instr->issue_end_cycle = numCycles;
		fRS.insert(instr);
		rob.insert(instr);
		dest.is_mapped = true;
		dest.map_value = instr->instructionId;

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

// TODO Change all pipeline functions to return void
bool Ex(Instruction* instruction)
{
	// in the driver function, we call this on every instruction in all Reservation Stations/ROB
	switch (instruction->op_code)
	{
	case beq:
	{
		if (instruction->qj == 0 && instruction->qk == 0 && !addFu.occupied)
		{
			instruction->ex_start_cycle = numCycles;
			addFu.dispatch(instruction);
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
				// update the branch target buffer
				// TODO port this code outside of the ex stage
				if (instruction->result == 1)
				{
					branchPredictor.table[instruction->btb_index].first = true;
					branchPredictor.table[instruction->btb_index].second = instruction->realized_instruction_target;
					if (!instruction->triggered_branch)
					{
						instruction->mispredict = true;
						instruction->branch_false_negative = true;
					}
				}
				else if (instruction->result == 0)
				{
					branchPredictor.table[instruction->btb_index].first = false;
					if (instruction->triggered_branch)
					{
						instruction->mispredict = true;
						instruction->branch_false_positive = true;
					}
				}
			}
		}
	}
		break;
	case bne:
	{
		if (instruction->qj == 0 && instruction->qk == 0 && !addFu.occupied)
		{
			instruction->ex_start_cycle = numCycles;
			addFu.dispatch(instruction);
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
				// update the branch target buffer
				// TODO port this code outside of the ex stage
				if (instruction->result == 1)
				{
					branchPredictor.table[instruction->btb_index].first = false;
					branchPredictor.table[instruction->btb_index].second = instruction->realized_instruction_target;
					if (instruction->triggered_branch)
					{
						instruction->mispredict = true;
						instruction->branch_false_positive = true;
					}
				}
				else if (instruction->result == 0)
				{
					branchPredictor.table[instruction->btb_index].first = true;
					if (!instruction->triggered_branch)
					{
						instruction->mispredict = true;
						instruction->branch_false_negative = true;
					}
				}
			}
		}
	}
		break;
	case nop:
		std::cout << "WARNING: Processing a NOP instruction with outdated logic" << std::endl;
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
	case add_i:
		// TODO change to for loop to handle multiple function units
		if (instruction->qj == 0 && !addFu.occupied)
		{
			// start the ex timer
			instruction->ex_start_cycle = numCycles;
			addFu.dispatch(instruction);
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
		break;
	case add_d:
		if (instruction->qj == 0 && instruction->qk == 0 && !fpFu.occupied)
		{
			instruction->ex_start_cycle = numCycles;
			fpFu.dispatch(instruction);
			return true;
		}

		else if (instruction == fpFu.instr)
		{
			double result = fpFu.next();
			if (!fpFu.occupied)
			{
				instruction->state = wb;
				instruction->result = result;
				instruction->ex_end_cycle = numCycles;
			}
			return true;
		}
		break;
	case sub_d:
		if (instruction->qj == 0 && instruction->qk == 0 && !fpFu.occupied)
		{
			instruction->ex_start_cycle = numCycles;
			fpFu.dispatch(instruction);
			return true;
		}

		else if (instruction == fpFu.instr)
		{
			double result = fpFu.next();
			if (!fpFu.occupied)
			{
				instruction->state = wb;
				instruction->result = result;
				instruction->ex_end_cycle = numCycles;
			}
			return true;
		}
		break;
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
			return true;
		}
		break;
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
		{
			if (instr->writeback_begin)
			{
				instr->writeback_begin = false;
				instr->writeback_start_cycle = numCycles;
			}
			if (!bus.occupied)
			{
				addRS.clear(instr);
				if (!bus.isEmpty())
				{
					bus.clear(instr);
				}
				bus.occupied = true;
				instr->writeback_end_cycle = numCycles;
				instr->state = commit;
				instr->commit_start_cycle = numCycles + 1;
			}
			else
			{
				bus.insert(instr);
			}
			// if we mispredicted, clear out everything
			if (instr->mispredict)
			{
				MispredictSquash(instr);

			}
		}
			break;
		case bne:
		{
			if (instr->writeback_begin)
			{
				instr->writeback_begin = false;
				instr->writeback_start_cycle = numCycles;
			}
			if (!bus.occupied)
			{
				addRS.clear(instr);
				if (!bus.isEmpty())
				{
					bus.clear(instr);
				}
				bus.occupied = true;
				instr->writeback_end_cycle = numCycles;
				instr->state = commit;
				instr->commit_start_cycle = numCycles + 1;
			}
			else
			{
				bus.insert(instr);
			}
			// if we mispredicted, clear out everything
			if (instr->mispredict)
			{
				MispredictSquash(instr);
			}
		}
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
				if (instr->op_code == add || instr->op_code == add_i || instr->op_code == sub)
				{
					addRS.clear(instr);
				}
				else if (instr->op_code == add_d || instr->op_code == sub_d || instr->op_code == mult_d)
				{
					fRS.clear(instr);
				}
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
				for (auto& instruction : fRS.table)
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
		if (((*rob.table.front()).state == commit) && ((*rob.table.front()).programLine == instr->programLine))
		{
			instr->state = null;
			rob.clear(instr);
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

		if (instr == rob.table[0] && !rob.hasCommited)
		{
			rob.hasCommited = true;
			int result = instr->result;
			int index = instr->dest;
			intRegFile.intRegFile[index] = result;
			intRat.table[index].is_mapped = false;
			intRat.table[index].value = result;
			instr->commit_end_cycle = numCycles;
			rob.clear(instr);
			outputInstructions.push_back(instr);
		}
		break;
	}
	case add_i:
	{
		if (instr->commit_begin)
		{
			instr->commit_begin = false;
			instr->commit_start_cycle = numCycles;
		}

		if (instr == rob.table[0] && !rob.hasCommited)
		{
			rob.hasCommited = true;
			int result = instr->result;
			int index = instr->dest;
			intRegFile.intRegFile[index] = result;
			intRat.table[index].is_mapped = false;
			intRat.table[index].value = result;
			instr->commit_end_cycle = numCycles;
			rob.clear(instr);
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

		if (instr == rob.table[0] && !rob.hasCommited)
		{
			rob.hasCommited = true;
			int result = instr->result;
			int index = instr->dest;
			intRegFile.intRegFile[index] = result;
			intRat.table[index].is_mapped = false;
			intRat.table[index].value = result;
			instr->commit_end_cycle = numCycles;
			rob.clear(instr);
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

		if (instr == rob.table[0] && !rob.hasCommited)
		{
			rob.hasCommited = true;
			double result = instr->result;
			int index = instr->dest;
			fpRegFile.fpRegFile[index] = result;
			fpRat.table[index].is_mapped = false;
			fpRat.table[index].value = result;
			instr->commit_end_cycle = numCycles;
			rob.clear(instr);
			outputInstructions.push_back(instr);
		}
		break;
	}
	case add_d:
	{
		if (instr->commit_begin)
		{
			instr->commit_begin = false;
			instr->commit_start_cycle = numCycles;
		}

		if (instr == rob.table[0] && !rob.hasCommited)
		{
			rob.hasCommited = true;
			double result = instr->result;
			int index = instr->dest;
			fpRegFile.fpRegFile[index] = result;
			fpRat.table[index].is_mapped = false;
			fpRat.table[index].value = result;
			instr->commit_end_cycle = numCycles;
			rob.clear(instr);
			outputInstructions.push_back(instr);
		}
		break;
	}
	case sub_d:
	{
		if (instr->commit_begin)
		{
			instr->commit_begin = false;
			instr->commit_start_cycle = numCycles;
		}

		if (instr == rob.table[0] && !rob.hasCommited)
		{
			rob.hasCommited = true;
			double result = instr->result;
			int index = instr->dest;
			fpRegFile.fpRegFile[index] = result;
			fpRat.table[index].is_mapped = false;
			fpRat.table[index].value = result;
			instr->commit_end_cycle = numCycles;
			rob.clear(instr);
			outputInstructions.push_back(instr);
		}
		break;
	}
	case bne:
	{
		if (instr->commit_begin)
		{
			instr->commit_begin = false;
			instr->commit_start_cycle = numCycles;
		}
		if (instr == rob.table[0] && !rob.hasCommited)
		{
			rob.hasCommited = true;
			int result = instr->result;
			instr->commit_end_cycle = numCycles;
			rob.clear(instr);
			outputInstructions.push_back(instr);
		}
	}
		break;
	case beq:
	{
		if (instr->commit_begin)
		{
			instr->commit_begin = false;
			instr->commit_start_cycle = numCycles;
		}
		if (instr == rob.table[0] && !rob.hasCommited)
		{
			rob.hasCommited = true;
			int result = instr->result;
			instr->commit_end_cycle = numCycles;
			rob.clear(instr);
			outputInstructions.push_back(instr);
		}
	}
	default:
	{
		throw "NO COMMIT IMPLEMENTED FOR INSTRUCTION YET";
	}
	}
}