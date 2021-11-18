// Main file for controlling the tomasulo algorithm project for ECE2162
// David Herr and Nathan Stodick
#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <deque>

#include "cpu_config.h"
#include "dataHoldingStructures.h"
#include "CDB.h"
#include "instruction.h"
#include "pipeline.h"
#include "structures/reorder_buffer.h"
#include "structures/reservation_station.h"
#include "structures/functional_units.h"
#include "structures/reorder_buffer.h"
#include "structures/reservation_station.h"
#include "structures/functional_units.h"
#include "structures/central_data_bus.h"
#include "structures/RegisterAliasingTable.h"

using namespace std;

// Global Variables
extern CPUConfig config;
extern RAT rat;
extern CDB dataBus;
extern AddReservationStation addRS;
extern FPReservationStation fRs;
extern ReorderBuffer rob2;
extern intReg intRegFile;
extern fpReg fpRegFile;
extern cpuMemory mainMem;
extern instructionBuffer instBuff;
extern AddFunctinalUnit addFu;
extern FPFunctionalUnit fpFu;
extern cdb bus;
extern ROM rom;
extern IntRegisterAliasingTable intRat;
extern FPRegisterAliasingTable fpRat;

// Non-hardware bookkeeping units
extern std::map<int, Instruction* > idMap;
extern int numCycles;

// Function prototypes
void programFSM(Instruction& instr);
void PrintCPUConfig(const CPUConfig& config);

void driver()
{
	// At the beginning of the simulation, file IO will be done first
	cout << "All the vars actually are read in" << endl;
	PrintCPUConfig(config);
	cout << "Finished Configuration printing" << endl;

	cout << "ROM Program memory set" << endl;
	while (true) {

		// ISSUE FETCH
		if (!rom.pc->end)
		{
			IssueFetch(rom.pc);
		}

		// ISSUE DECODE
		if (instBuff.inst.size() != 0)
		{
			auto& bufferHead = instBuff.inst[0];
			if (bufferHead->just_fetched == true)
			{
				bufferHead->just_fetched = false;
			}
			else
			{
				Issue(bufferHead);
				if (bufferHead->state == ex)
				{
					instBuff.clear(bufferHead);
					bufferHead->ex_begin = true;
				}
			}
		}

		// ROB will be empty at the beginning of the program and end
		// We still want to count a cycle
		if (rob2.isEmpty())
		{
			if (numCycles > 0)
			{
				numCycles++;
				break;
			}
			numCycles++;
			continue;
		}

		for (auto instr : rob2.table)
		{
			switch (instr->state)
			{
			case ex:
				if (instr->ex_begin)
				{
					instr->ex_begin = false;
					break;
				}
				else
				{
					Ex(instr);
					break;
				}
			case wb:
				WriteBack(instr);
				bus.occupied = false;
				break;
			case commit:
				Commit(instr);
				break;
			}
		}
		rob2.hasCommited = false;
		numCycles++;


		
		//// Step through every instruction to check and make sure 
		//for (int i = 0; i < instBuff.getNumInsts(); i++) {
		//	if (instBuff.inst[i]->state != null) {
		//		cout << "Stepping through the pipeline" << endl;
		//		cout << "State of the currently executing instruction is: " << instBuff.inst[i]->state << endl;
		//		cout << "ID of the current executing instruction is: " << instBuff.inst[i]->instructionId << endl;
		//		programFSM(*instBuff.inst[i]);
		//	}
		//}
		//cout << "Instruction line of top instruction in ROB = " << (*rob2.table.front()).programLine << endl;
		//// When the simulation is done, the ROB will be empty, and the curinst will be equal to the max number of insts. 
		///*if (rom.pc->end && rob2.table.empty()) {
		//	break;
		//}*/
	}
}

//void programFSM(Instruction& instr) {
//	switch (instr.state) {
//	case issue:
//		Issue(instr);
//		break;
//	case ex:
//		Ex(instr);
//		break;
//	case mem:
//		Mem(instr);
//		break;
//	case wb:
//		WriteBack(instr);
//		break;
//	case commit:
//		Commit(instr);
//		break;
//	default:
//		break;
//	}
//}

