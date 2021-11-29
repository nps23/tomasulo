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
#include "structures/BranchPredictor.h"

using namespace std;

// Global Variables
extern CPUConfig config;
extern RAT rat;
extern CDB dataBus;
extern AddReservationStation addRS;
extern FPReservationStation fRS;
extern ReorderBuffer rob;
extern intReg intRegFile;
extern fpReg fpRegFile;
extern cpuMemory mainMem;
extern instructionBuffer instBuff;
extern AddFunctinalUnit addFu;
extern FPAddFunctionalUnit fpFu;
extern cdb bus;
extern ROM rom;
extern IntRegisterAliasingTable intRat;
extern FPRegisterAliasingTable fpRat;
extern BranchPredictor branchPredictor;

// Non-hardware bookkeeping units
extern std::map<int, Instruction* > idMap;
extern int numCycles;

// Pipeline controllers
extern bool stall_fetch;

// Function prototypes
void programFSM(Instruction& instr);
void PrintCPUConfig(const CPUConfig& config);

void driver()
{
	//PrintCPUConfig(config);
	while (true) {

		if (numCycles == 20)
			break;
		// ISSUE FETCH
		if (!rom.pc->program_end && !stall_fetch)
		{
			IssueFetch(rom.pc);
		}
		
		numCycles++;

		// ISSUE DECODE
		if (instBuff.inst.size() != 0)
		{
			auto& bufferHead = instBuff.inst[0];
			IssueDecode(bufferHead);
		}

		// ROB will be empty at the beginning of the program and end
		// We still want to count a cycle
		if (rob.isEmpty())
		{
			if (numCycles > 0)
			{
				// if we stalled on a misbranch until the ROB was clear, resume fetching
				if (stall_fetch)
				{
					for (auto& entry : intRat.table)
					{
						entry.is_mapped = false;
					}
					for (auto& entry : fpRat.table)
					{
						entry.is_mapped = false;
					}
					stall_fetch = false;
				}
				numCycles++;
				break;
			}
			numCycles++;
			continue;
		}

		// ISSUE EX, WRITEBACK, MEM, AND COMMIT
		for (auto& instr : rob.table)
		{
			switch (instr->state)
			{
			case issue:
				if (instr->issued) 
				{
					instr->state = ex;
				}
				break;
			case ex:
				/*if (instr->ex_begin)
				{
					instr->ex_begin = false;
					break;
				}
				else
				{
					Ex(instr);
					break;
				}*/
				Ex(instr);
				break;
			case wb:
				WriteBack(instr);
				bus.occupied = false;
				break;
			case commit:
				Commit(instr);
				break;
			}
		}
		rob.hasCommited = false;
		numCycles++;
	}
}