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
#include "instruction.h"
#include "pipeline.h"
#include "structures/reorder_buffer.h"
#include "structures/reservation_station.h"
#include "structures/functional_units.h"

using namespace std;

// Global Variables
CPUConfig config = ParseInput(input_file);
ROB rob(config);
RAT rat;
timingDiagram output(config.program.size());
CDB dataBus;
AddReservationStation addRS(config.fu_fp_adder[0]);
FPReservationStation fRs(config.fu_fp_mult[0]);
ReorderBuffer rob2(config.rob_entries);
intReg intRegFile(config);
fpReg fpRegFile(config);
cpuMemory mainMem(config);
instructionBuffer instBuff;
AddFunctinalUnit addFu(config.fu_fp_adder[1]);
FPFunctionalUnit fpFu(config.fu_fp_mult[1]);

int numCycles = 0;

// Function prototypes
void programFSM(Instruction& instr);
void PrintCPUConfig(const CPUConfig& config);

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
	instructionBuffer instBuff(config.program.size());
  
	// Set the first column of the output 
	for(int i = 0; i < output.numLines; i++){
			output.tDiag[i][0] = i;
	}

	while (true) {
		
		// Issue a new instruction if there are still instructions to read in
		if(instBuff.curInst < instBuff.maxInstructions){
			// If the current instruction is a branch, stop grabbing instruction to avoid speculative branching
			if(((instBuff.inst[instBuff.curInst].op_code != bne) || (instBuff.inst[instBuff.curInst].op_code != beq)) && (instBuff.inst[instBuff.curInst].state != wb)){
				instBuff.curInst++;
				instBuff.inst[instBuff.curInst].state = issue;
			}
		}
		
		// Step through every instruction to check and make sure 
		for(int i = 0; i < instBuff.maxInstructions; i++){
			if(instBuff.inst[i].state != null){
				programFSM(instBuff.inst[i]);
			}
		}

		// When the simulation is done, the ROB will be empty, and the curinst will be equal to the max number of insts. 
		if((instBuff.maxInstructions == instBuff.curInst) && rob.entries == rob.freeEntries){
			break;
		}
		numCycles++;
	}
	return 0;
}

//void programFSM(Instruction& instr){
//	CDBEntry wbEntry;
//	switch(instr.state){
//		case issue:
//			switch(instr.op_code){
//				case nop:
//					if(!rob.checkFull()){
//						rob.insert(nop, "NULL", -1);
//						instr.state = ex;
//						output.tDiag[instr.programLine][1] = numCycles;
//					}
//					break;
//				case add:
//					if(!(rob.checkFull()) && (!intRS.checkFull())){
//						rob.insert(add, instr.dest, -1);
//						int valsReady;
//						
//						/*switch valsReady {
//							case 0:
//								//intRS.insertOp(add, );
//								break;
//							case 1:
//								break;
//							case 2:
//								break;
//							case 3:
//								break;
//							default:
//								break;
//						}*/
//						instr.state = ex;
//						output.tDiag[instr.programLine][1] = numCycles;
//					}
//					break;
//				case add_i:
//					if(!rob.checkFull() && !intRS.checkFull()){
//						rob.insert(add_i, instr.dest, -1);
//						//intRS.insertOp(add_i, );
//						instr.state = ex;
//						output.tDiag[instr.programLine][1] = numCycles;
//					}
//					break;
//				case sub:
//					if (!rob.checkFull() && !intRS.checkFull()){
//						rob.insert(sub, instr.dest, -1);
//						//intRS.insertOp(sub, );
//						instr.state = ex;
//						output.tDiag[instr.programLine][1] = numCycles;
//					}
//					break;
//				default:
//					break;
//			}
//			break;
//		case ex:
//			switch(instr.op_code){
//				case nop:
//					instr.state = wb;
//					output.tDiag[instr.programLine][2] = numCycles;
//					break;
//				case add:
//					instr.state = wb;
//					output.tDiag[instr.programLine][4] = numCycles;
//					break;
//				case add_i:
//					instr.state = wb;
//					output.tDiag[instr.programLine][4] = numCycles;
//					break;
//				case sub:
//					instr.state = wb;
//					output.tDiag[instr.programLine][4] = numCycles;
//					break;
//				default:
//				break;
//			}
//			break;
//		case mem:
//			switch(instr.op_code){
//				default:
//				break;
//			}
//			break;
//		case wb:
//			switch(instr.op_code){
//				case nop:
//					instr.state = commit;
//					output.tDiag[instr.programLine][4] = numCycles;
//					break;
//				case add:
//					wbEntry.value = instr.result;
//					wbEntry.programLine = instr.programLine;
//					wbEntry.dest = instr.dest;
//					dataBus.cdbBuffer.emplace(wbEntry);
//					if(dataBus.cdbBuffer.top().programLine == instr.programLine){
//						// Write back to ROB
//						for(int i = 0; i < rob.maxEntries; i++){
//							if(rob.destValue[i] == instr.dest){
//								rob.valueField[i] = instr.result;
//								break;
//							}
//						}
//						// Write back to RS's
//						for(int i = 0; i < intRS.maxRS; i++){
//							if(intRS.qj[i] == instr.dest){
//								intRS.vj[i] = instr.result;
//								break;
//							}
//							if(intRS.qk[i] == instr.dest){
//								intRS.vk[i] = instr.result;
//								break;
//							}
//						}
//						for(int i = 0; i < fpRS.maxRS; i++){
//							if(fpRS.qj[i] == instr.dest){
//								fpRS.vj[i] = instr.result;
//								break;
//							}
//							if(fpRS.qk[i] == instr.dest){
//								fpRS.vk[i] = instr.result;
//								break;
//							}
//						}
//						dataBus.cdbBuffer.pop();
//						instr.state = commit;
//						output.tDiag[instr.programLine][4] = numCycles;
//					}
//					break;
//				case add_i:
//					wbEntry.value = instr.result;
//					wbEntry.programLine = instr.programLine;
//					wbEntry.dest = instr.dest;
//					dataBus.cdbBuffer.emplace(wbEntry);
//					if(dataBus.cdbBuffer.top().programLine == instr.programLine){
//						// Write back to ROB
//						for(int i = 0; i < rob.maxEntries; i++){
//							if(rob.destValue[i] == instr.dest){
//								rob.valueField[i] = instr.result;
//								break;
//							}
//						}
//						// Write back to RS's
//						for(int i = 0; i < intRS.maxRS; i++){
//							if(intRS.qj[i] == instr.dest){
//								intRS.vj[i] = instr.result;
//								break;
//							}
//							if(intRS.qk[i] == instr.dest){
//								intRS.vk[i] = instr.result;
//								break;
//							}
//						}
//						for(int i = 0; i < fpRS.maxRS; i++){
//							if(fpRS.qj[i] == instr.dest){
//								fpRS.vj[i] = instr.result;
//								break;
//							}
//							if(fpRS.qk[i] == instr.dest){
//								fpRS.vk[i] = instr.result;
//								break;
//							}
//						}
//						dataBus.cdbBuffer.pop();
//						instr.state = commit;
//						output.tDiag[instr.programLine][4] = numCycles;
//					}
//					break;
//				case sub:
//					wbEntry.value = instr.result;
//					wbEntry.programLine = instr.programLine;
//					wbEntry.dest = instr.dest;
//					dataBus.cdbBuffer.emplace(wbEntry);
//					if(dataBus.cdbBuffer.top().programLine == instr.programLine){
//						// Write back to ROB
//						for(int i = 0; i < rob.maxEntries; i++){
//							if(rob.destValue[i] == instr.dest){
//								rob.valueField[i] = instr.result;
//								break;
//							}
//						}
//						// Write back to RS's
//						for(int i = 0; i < intRS.maxRS; i++){
//							if(intRS.qj[i] == instr.dest){
//								intRS.vj[i] = instr.result;
//								break;
//							}
//							if(intRS.qk[i] == instr.dest){
//								intRS.vk[i] = instr.result;
//								break;
//							}
//						}
//						for(int i = 0; i < fpRS.maxRS; i++){
//							if(fpRS.qj[i] == instr.dest){
//								fpRS.vj[i] = instr.result;
//								break;
//							}
//							if(fpRS.qk[i] == instr.dest){
//								fpRS.vk[i] = instr.result;
//								break;
//							}
//						}
//						dataBus.cdbBuffer.pop();
//						instr.state = commit;
//						output.tDiag[instr.programLine][4] = numCycles;
//					}
//					break;
//				default:
//				break;
//			}
//			break;
//		case commit:
//			switch(instr.op_code){
//				case nop:
//					rob.commit();
//					instr.state = null;
//					output.tDiag[instr.programLine][5] = numCycles;
//					break;
//				case add:
//					rob.commit();
//					instr.state = null;
//					output.tDiag[instr.programLine][5] = numCycles;
//					break;
//				case add_i:
//					rob.commit();
//					instr.state = null;
//					output.tDiag[instr.programLine][5] = numCycles;
//					break;
//				case sub:
//					rob.commit();
//					instr.state = null;
//					output.tDiag[instr.programLine][5] = numCycles;
//					break;
//				default:
//				break;
//			}
//			break;
//		// Handles Null case
//		default:
//			// If the instruction state is null, ignore the instruction.
//		break;
//	}
//}


// print the status of the CPU for debug
void PrintCPUConfig(const CPUConfig& config)
{
	std::cout << "Integer adder: \n";
	std::cout << "\tnumber of reservation stations: ";
	std::cout << config.fu_integer_adder[0];
	std::cout << "\n \tcycles in ex: ";
	std::cout << config.fu_integer_adder[1];
	std::cout << "\n \tnumber of function units: ";
	std::cout << config.fu_integer_adder[2];
	std::cout << std::endl;

	std::cout << "Floating point adder: \n";
	std::cout << "\tnumber of reservation stations: ";
	std::cout << config.fu_fp_adder[0];
	std::cout << "\n \tcycles in ex: ";
	std::cout << config.fu_fp_adder[1];
	std::cout << "\n \tnumber of function units: ";
	std::cout << config.fu_fp_adder[2];
	std::cout << std::endl;

	std::cout << "Floating point multiplier: \n";
	std::cout << "\tnumber of reservation stations: ";
	std::cout << config.fu_fp_mult[0];
	std::cout << "\n \tcycles in ex: ";
	std::cout << config.fu_fp_mult[1];
	std::cout << "\n \tnumber of function units: ";
	std::cout << config.fu_fp_mult[2];
	std::cout << std::endl;

	std::cout << "Load store unit: \n";
	std::cout << "\tnumber of reservation stations: ";
	std::cout << config.fu_load_store[0];
	std::cout << "\n \tcycles in ex: ";
	std::cout << config.fu_load_store[1];
	std::cout << "\n \tnumber of function units: ";
	std::cout << config.fu_load_store[2];
	std::cout << std::endl;

	std::cout << "ROB entries: " << config.rob_entries << std::endl;
	std::cout << "CDB buffer entires: " << config.cdb_buffer_entries << std::endl;


	std::cout << "Inital r register states:";
	for (const auto& elem : config.r_register_map)
	{
		std::cout << "\n \t" << elem.first << " " << elem.second;
	}
	std::cout << std::endl;

	std::cout << "Inital f register states:";
	for (const auto& elem : config.f_register_map)
	{
		std::cout << "\n \t" << elem.first << " " << elem.second;
	}
	std::cout << std::endl;

	std::cout << "Program:" << std::endl;
	for (const auto& instruction : config.program)
	{
		switch (instruction.op_code)
		{
		case 0:
			std::cout << "NOP" << std::endl;
			break;
		case 1:
			std::cout << "ld" << std::endl;
			break;
		case 2:
			std::cout << "sd" << std::endl;
			break;
		case 3:
			std::cout << "beq" << std::endl;
			break;
		case 4:
			std::cout << "bne" << std::endl;
			break;
		case 5:
			std::cout << "add" << std::endl;
			break;
		case 6:
			std::cout << "add_d" << std::endl;
			break;
		case 7:
			std::cout << "add_i" << std::endl;
			break;
		case 8:
			std::cout << "sub" << std::endl;
			break;
		case 9:
			std::cout << "sub_d" << std::endl;
			break;
		case 10:
			std::cout << "mult_d" << std::endl;
			break;
		default:
			std::cout << "Bad instruction. Something went wrong" << std::endl;
			break;
		}
	}
	std::cout << std::endl;

	std::cout << "Memory" << std::endl;
	for (const auto& address : config.memory)
	{
		std::cout << "Address " << address.first << ": " << address.second << std::endl;
	}
}
