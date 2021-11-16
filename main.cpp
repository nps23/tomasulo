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

// Non-hardware bookkeeping units
std::map<int, Instruction* > idMap;
int numCycles = 0;

// Function prototypes
void programFSM(Instruction& instr);
void PrintCPUConfig(const CPUConfig& config);

int main()
{
	// At the beginning of the simulation, file IO will be done first

	PrintCPUConfig(config);
	
	// Next, using the information read in with the file IO, configure the simulation

	while (true) {
		
		// Issue a new instruction if there are still instructions to read in
		if(instBuff.curInst < instBuff.getNumInsts()){
			// If the current instruction is a branch, stop grabbing instruction to avoid speculative branching
			if(((instBuff.inst[instBuff.curInst].op_code != bne) || (instBuff.inst[instBuff.curInst].op_code != beq)) && (instBuff.inst[instBuff.curInst].state != wb)){
				instBuff.curInst++;
				instBuff.inst[instBuff.curInst].state = issue;
			}
		}
		
		// Step through every instruction to check and make sure 
		for(int i = 0; i < instBuff.getNumInsts(); i++){
			if(instBuff.inst[i].state != null){
				programFSM(instBuff.inst[i]);
			}
		}

		// When the simulation is done, the ROB will be empty, and the curinst will be equal to the max number of insts. 
		if((instBuff.getNumInsts() == instBuff.curInst) && rob2.isEmpty()){
			break;
		}
		numCycles++;
	}
	
	// Finally create the timing diagram
	timingDiagram output(instBuff.inst.size());
	for(unsigned int i = 0; i < instBuff.inst.size(); i++){
		output.tDiag[i][1] = instBuff.inst[i].issue_end_cycle;
		output.tDiag[i][2] = instBuff.inst[i].ex_end_cycle;
		if(instBuff.inst[i].op_code == ld){
			output.tDiag[i][3] = instBuff.inst[i].mem_end_cycle;
		}
		output.tDiag[i][4] = instBuff.inst[i].writeback_end_cycle;
		output.tDiag[i][5] = instBuff.inst[i].commit_end_cycle;
	}
	
	// Print out the timing diagram and the register contents to a file
	ofstream outFile("output.txt");
	if(outFile.is_open()){
		outFile << "\t\tIS EX MM WB CM" << endl;
		for(unsigned int i = 0; i < instBuff.inst.size(); i++){
			switch(instBuff.inst[i].op_code){
				case nop:
					outFile << "nop\t\t" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case ld:
					outFile << "ld " << "F" << instBuff.inst[i].dest << ", " << instBuff.inst[i].offset << "(" << instBuff.inst[i].f_ls_register_operand << ")\t\t" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case sd:
					outFile << "sd " << "F" << instBuff.inst[i].dest << ", " << instBuff.inst[i].offset << "(" << instBuff.inst[i].f_ls_register_operand << ")\t\t" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case beq:
					outFile << "beq " << "R" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case bne:
					outFile << "bne " << "R" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case add:
					outFile << "add " << "" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case add_d:
					outFile << "add_d " << "" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case add_i:
					outFile << "add_i " << "" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case sub:
					outFile << "sub " << "" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case sub_d:
					outFile << "sub_d " << "" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
				case mult_d:
					outFile << "mult_d " << "" << output.tDiag[i][1] << output.tDiag[i][2] << output.tDiag[i][3] << output.tDiag[i][4] << output.tDiag[i][5];
					break;
			}
		}
	}else{
		cout << "ERROR: FILE OPEN FAILURE" << endl;
	}
	return 0;
}

void programFSM(Instruction& instr){
	switch(instr.state){
		case issue:
			//Issue(instr);
			break;
		case ex:
			//Ex(instr);
			break;
		case mem:
			//Mem(instr);
			break;
		case wb:
			//WriteBack(instr);
			break;
		case commit:
			//Commit(instr);
			break;
		default:
			break;
	}
}


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
