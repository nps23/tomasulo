// Main file for controlling the tomasulo algorithm project for ECE2162
// David Herr and Nathan Stodick
#include <iostream>
#include <map>
#include <string>
#include <queue>
#include "cpu_config.h"
#include "input_parser.h"
#include "dataHoldingStructures.h"
#include "CDB.h"
#include "program.h"

using namespace std;
CPUConfig config = ParseInput(input_file);
ROB rob(config);

//void programFSM(Instruction& instr);

int main()
{
	// At the beginning of the simulation, file IO will be done first
	PrintCPUConfig(config);
	
	// Next, using the information read in with the file IO, configure the simulation

	/*INSERT CODE TO CONFIGURE AND INITILIZE HERE*/ 
	intReg intRegFile(config);
	fpReg fpRegFile(config);
	cpuMemory mainMem(config);
	ROB rob(config);
	instructionBuffer instBuff;

	timingDiagram output(config.program.size());
	int numCycles = 0;

	// Now enter the loop

	// The loop will run until a break condition is met
	
	// TO DO: CDB: Queue class with arbitrate function that can reorder based on program line every cycle. 
	// DONE: RAT, Store Queue, Memory, RS For the RS's, we must use a mapping.
	// CHANGE ARF's to use a mapping instead now, Instruction Buffer, ROB,
	while (true) {
		
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

//void programFSM(Instruction& instr){
//	switch(instr.state){
//		case issue:
//			switch(instr.op_code){
//				case nop:
//					if(rob.checkFull() == false){
//						rob.insert(instr.op_code(nop, "NULL", 0));
//						instr.state = ex;
//						 INSERT INTO TIMING DIAGRAM
//					}
//				default:
//			}
//			break;
//		case ex:
//			switch(instr.op_code){
//				case nop:
//					instr.state = wb;
//					 INSERT INTO TIMING DIAGRAM
//				default:
//			}
//			break;
//		case mem:
//			switch(instr.op_code){
//				default:
//			}
//			break;
//		case wb:
//			switch(instr.op_code){
//				case nop:
//					instr.state = commit;
//					 INSERT INTO TIMING DIAGRAM
//				default:
//			}
//			break;
//		case commit:
//			switch(instr.op_code){
//				case nop:
//					rob.commit();
//					instr.state = null;
//					 INSERT INTO TIMING DIAGRAM
//				default:
//			}
//			break;
//		 Handles Null case
//		default:
//			 If the instruction state is null, ignore the instruction.
//	}
//}
