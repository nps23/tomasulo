#pragma once

#include <iostream>
#include "program.h"
#include "data_holding_structures.h"
#include "input_parser.h"
#include "structures/reorder_buffer.h"

bool InstructionFetch(const Instruction& inst)
{
	return true;
	// since we are abstracting this away, the only thing this needs to do is branch prediction
}

bool InstructionDecode(const Instruction& inst)
{
	return true;
	// just ensure we are branch predicting first
}

bool Execute(const Instruction& inst)
{
	return true;
	// see specifics for each case
}

bool WriteBack(const Instruction& inst)
{
	return true;
}



int Simulate()
{
	// Parse the CPU configuration settings from a text file and load them into a struct
	CPUConfig config = ParseInput(input_file);
	
	//PrintCPUConfig(config);

	// Next, using the information read in with the file IO, configure the simulation
	intReg intRegFile(config);
	fpReg fpRegFile(config);
	cpuMemory mainMem(config);
	ROB rob(config);
	RS adderRS(config.fu_integer_adder[0]);
	RS fpRS(config.fu_fp_mult[0]);
	RAT rat;
	
	// Instruction buffer considered to have sufficient space to hold everything
	instructionBuffer instBuff(config);

	timingDiagram output(config.program.size());
	int numCycles = 0;

	// Now enter the loop

	// The loop will run until a break condition is met

	// TO DO: CDB: Queue class with arbitrate function that can reorder based on program line every cycle. 
	// DONE: RAT, Store Queue, Memory, RS For the RS's, we must use a mapping.
	// CHANGE ARF's to use a mapping instead now, Instruction Buffer, ROB,
	while (true) {

		// lets just start simulating, and see where we run into issues.
		bool instruction_issued = false;
		if (!instBuff.empty())
		{
			Instruction inst = instBuff.inst[0];
			std::cout << inst.op_code << std::endl;
			switch (inst.op_code)
			{
			case nop:
				instruction_issued = true;
				break;
			case ld:
				instruction_issued = true;
				break;
			case sd:
				instruction_issued = true;
				break;
			case beq:
				instruction_issued = true;
				break;
			case bne:
				instruction_issued = true;
				break;
			case add:
				instruction_issued = true;
				break;
			case add_d:
				if (!fpRS.isFull() && !rob.isFull())
				{
					int l_operand = inst.f_left_operand;
					int r_operand = inst.f_right_operand;
					int dest = inst.dest;

					instruction_issued = true;
				}
				else
				{
					// stall();
				}
				break;
			case add_i:
				instruction_issued = true;
				break;
			case sub:
				instruction_issued = true;
				break;
			case sub_d:
				if (!fpRS.isFull())
				{
					// issue instruction, update state
				}
				instruction_issued = true;
				break;
			case mult_d:
				if (!fpRS.isFull())
				{

				}
				instruction_issued = true;
				break;
			}

			if (instruction_issued)
			{
				instBuff.pop_front();
			}
		}
		
		//if(instBuff.currentInst < ){
		//	
		//}
		//for(int i = 0; i < currentInst; i++){
		//	programFSM(instBuff.inst);
		//}

		// This loop will handle the cycling for the simulation
		// For loops, we will need to implement a static instruction buffer to use for reference to loop back to previous lines of code. 
		// The loop will go through all of the neccessary steps to update things accordingly. 
		// First, fetch new instruction if it is available
		/*if( instructionBuffer != empty){
			nextInst = insttructionBuffer[i];
			* i++;
		}*/

		// Next decode the previous instruction
		/*IDEA: Read OP code, load reservation station, check dependencies and flag accordingly, finally load into ROB*/

		/* IDEA: loop through every RS to see if they can begin execution and start a timer for how many cycles it should take
		 If the number of cycles equals the number for the instruction, flag the ROB for completion*/

		 // MEM: For memory, if there is a load instruction ready, search memory for value and 

		 // WB: load any results to RS and ROB waiting for it, arbitrate, first come first served based on program line, check ROB ready to commit if complete.

		 // COMMIT: Write store to memory if instruction up is a store and complete it, otehrwise clear the next row of the ROB. 


		 /* IDEA for how cycling would be stored. A M by 6 array where M is the number of instructions,
		 the first column is the instruction number, and each following column represents a pipeline step.
		 Each step would be updated when the instruction starts the cycle, with the exceptions being execution and memory.
		 Execution and memory columns will be written when the instruction exits these steps.*/

		 // Current idea for breaking the cycling loop
		 /*
		 if(instructionBuffer == empty && ROB == empty){
			 break;
		 }
		 */
		if (numCycles == 10) {
			break;
		}
		numCycles++;
	}
	return 0;
}