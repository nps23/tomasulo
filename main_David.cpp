// Main file for controlling the tomasulo algorithm project for ECE2162
// David Herr and Nathan Stodick
#include "registerFiles.h"
int main() {

	// At the beginning of the simulation, file IO will be done first

	/*INSERT FILE IO CALLS HERE*/

	// Next, using the information read in with the file IO, configure the simulation

	/*INSERT CODE TO CONFIGURE AND INITILIZE HERE*/
	intReg intRegFile();
	fpReg fpRegFile();
	int numCycles = 0;

	// Now enter the loop

	// The loop will run until a break condition is met
	
	// For the RS's, we must use a mapping. CHANGE ARF's to use a mapping instead now. 
	// TO DO: CDB: Queue class with arbitrate function that can reorder based on program line every cycle. 
	// ROB, RS, Store Queue, Memory
	
	while (true) {

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
