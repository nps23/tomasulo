#pragma once

// Nate's version of "main", to avoid merge conflicts.

#include <iostream>
#include "program.h"
#include "data_holding_structures.h"
#include "input_parser.h"


int Simulate()
{
	// Parse the CPU configuration settings from a text file and load them into a struct
	CPUConfig config = ParseInput(input_file);
	
	PrintCPUConfig(config);

	// Next, using the information read in with the file IO, configure the simulation
	intReg intRegFile(config);
	fpReg fpRegFile(config);
	cpuMemory mainMem(config);
	ROB rob(config);
	RS adderRS(config.fu_integer_adder[0]);
	RS fpRS(config.fu_fp_mult[0]);
	
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
		if (!instBuff.empty())
		{
			Instruction inst = instBuff.inst[0];
			switch (inst.op_code)
			{
			case nop:
				break;
			case ld_fa:
				break;
			case sd_fa:
				break;
			case beq:
				break;
			case bne:
				break;
			case add:
				break;
			case add_d:
				if (!fpRS.isFull())
				{
					// issue instruction, update state.
				}
				break;
			case add_i:
				break;
			case sub:
				break;
			case sub_d:
				if (!fpRS.isFull())
				{
					// issue instruction, update state
				}
				break;
			case mult_d:
				if (!fpRS.isFull())
				{
					// parse issue instruction, update the state

				}
				break;
			}
			
			// only pop the instruction from the vector if we can issue it 
			// if (issue succeeded) -> pop off queue
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