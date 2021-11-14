#pragma once

#include "structures/reorder_buffer.h"
#include <string>

enum OpCode {
	nop = 0,
	ld,
	sd,
	beq,
	bne,
	add,
	add_d,
	add_i,
	sub,
	sub_d,
	mult_d
};

enum PipelineState {
	null = 0,
	issue,
	ex,
	mem,
	wb,
	commit,
	stop
};

class Instruction {
public:
	// metadata associatd with the instruction itself
	OpCode op_code;
	PipelineState state;

	int r_right_operand;
	int r_left_operand;
	int f_right_operand;
	int f_left_operand;
	int dest;

	double result;

	int programLine;

	// ROB fields
	int robEntry;
	int instType;
	std::string destValue;
	bool rob_busy;

	// RS fields
	int rsEntry;
	std::string robIndex;
	double vj;
	double vk;
	int qj;  // ROB entry which holds the value we are looking for
	int qk;
	bool rs_busy;

	// Load store queue
	int offset;
	int immediate;
	int r_ls_register_operand;
	int f_ls_register_operand;

	int pipelineTimer;

	// Timing diagram values
	int issue_start_cycle = 0;
	int issue_end_cycle = 0;
	int ex_start_cycle = 0;
	int ex_end_cycle = 0;
	int writeback_start_cycle = 0;
	int writeback_end_cycle = 0;
};

void Issue(Instruction& inst, ReorderBuffer& rob, ReservationStation& intRS, ReservationStation& fpRS, int numCycles, timingDiagram output)
{
	// switch on op code
	// if double words:
	switch(inst.op_code){
		case nop:
			if(!rob.checkFull()){
				rob.insert(nop, "NULL", -1);
				instr.state = ex;
				output.tDiag[instr.programLine][1] = numCycles;
			}
			break;
		case add:
			if(!(rob.checkFull()) && (!intRS.checkFull())){
				rob.insert(add, instr.dest, -1);
				instr.state = ex;
				output.tDiag[instr.programLine][1] = numCycles;
			}
			break;
		case add_i:
			if(!rob.checkFull() && !intRS.checkFull()){
				rob.insert(add_i, instr.dest, -1);
				//intRS.insertOp(add_i, );
				instr.state = ex;
				output.tDiag[instr.programLine][1] = numCycles;
			}
			break;
		case sub:
			if (!rob.checkFull() && !intRS.checkFull()){
				rob.insert(sub, instr.dest, -1);
				//intRS.insertOp(sub, );
				instr.state = ex;
				output.tDiag[instr.programLine][1] = numCycles;
			}
			break;
		default:
			break;
	}
}


void Ex(Instruction& inst, ReorderBuffer& rob, ReservationStation& intRS, ReservationStation& fpRS, int numCycles, timingDiagram output){
	switch(instr.op_code){
		case nop:
			instr.state = wb;
			output.tDiag[instr.programLine][2] = numCycles;
			break;
		case add:
			instr.state = wb;
			output.tDiag[instr.programLine][4] = numCycles;
			break;
		case add_i:
			instr.state = wb;
			output.tDiag[instr.programLine][4] = numCycles;
			break;
		case sub:
			instr.state = wb;
			output.tDiag[instr.programLine][4] = numCycles;
			break;
		default:
		break;
	}
}

void Mem(Instruction& inst, ReorderBuffer& rob, ReservationStation& intRS, ReservationStation& fpRS, int numCycles, timingDiagram output){
	switch(instr.op_code){
		default:
		break;
	}
}

void WriteBack(Instruction& inst, ReorderBuffer& rob, ReservationStation& intRS, ReservationStation& fpRS, int numCycles, timingDiagram output){
switch(instr.op_code){
				case nop:
					instr.state = commit;
					output.tDiag[instr.programLine][4] = numCycles;
					break;
				case add:
					wbEntry.value = instr.result;
					wbEntry.programLine = instr.programLine;
					wbEntry.dest = instr.dest;
					dataBus.cdbBuffer.emplace(wbEntry);
					if(dataBus.cdbBuffer.top().programLine == instr.programLine){
						// Write back to ROB
						for(int i = 0; i < rob.maxEntries; i++){
							if(rob.destValue[i] == instr.dest){
								rob.valueField[i] = instr.result;
								break;
							}
						}
						// Write back to RS's
						for(int i = 0; i < intRS.maxRS; i++){
							if(intRS.qj[i] == instr.dest){
								intRS.vj[i] = instr.result;
								break;
							}
							if(intRS.qk[i] == instr.dest){
								intRS.vk[i] = instr.result;
								break;
							}
						}
						for(int i = 0; i < fpRS.maxRS; i++){
							if(fpRS.qj[i] == instr.dest){
								fpRS.vj[i] = instr.result;
								break;
							}
							if(fpRS.qk[i] == instr.dest){
								fpRS.vk[i] = instr.result;
								break;
							}
						}
						dataBus.cdbBuffer.pop();
						instr.state = commit;
						output.tDiag[instr.programLine][4] = numCycles;
					}
					break;
				case add_i:
					wbEntry.value = instr.result;
					wbEntry.programLine = instr.programLine;
					wbEntry.dest = instr.dest;
					dataBus.cdbBuffer.emplace(wbEntry);
					if(dataBus.cdbBuffer.top().programLine == instr.programLine){
						// Write back to ROB
						for(int i = 0; i < rob.maxEntries; i++){
							if(rob.destValue[i] == instr.dest){
								rob.valueField[i] = instr.result;
								break;
							}
						}
						// Write back to RS's
						for(int i = 0; i < intRS.maxRS; i++){
							if(intRS.qj[i] == instr.dest){
								intRS.vj[i] = instr.result;
								break;
							}
							if(intRS.qk[i] == instr.dest){
								intRS.vk[i] = instr.result;
								break;
							}
						}
						for(int i = 0; i < fpRS.maxRS; i++){
							if(fpRS.qj[i] == instr.dest){
								fpRS.vj[i] = instr.result;
								break;
							}
							if(fpRS.qk[i] == instr.dest){
								fpRS.vk[i] = instr.result;
								break;
							}
						}
						dataBus.cdbBuffer.pop();
						instr.state = commit;
						output.tDiag[instr.programLine][4] = numCycles;
					}
					break;
				case sub:
					wbEntry.value = instr.result;
					wbEntry.programLine = instr.programLine;
					wbEntry.dest = instr.dest;
					dataBus.cdbBuffer.emplace(wbEntry);
					if(dataBus.cdbBuffer.top().programLine == instr.programLine){
						// Write back to ROB
						for(int i = 0; i < rob.maxEntries; i++){
							if(rob.destValue[i] == instr.dest){
								rob.valueField[i] = instr.result;
								break;
							}
						}
						// Write back to RS's
						for(int i = 0; i < intRS.maxRS; i++){
							if(intRS.qj[i] == instr.dest){
								intRS.vj[i] = instr.result;
								break;
							}
							if(intRS.qk[i] == instr.dest){
								intRS.vk[i] = instr.result;
								break;
							}
						}
						for(int i = 0; i < fpRS.maxRS; i++){
							if(fpRS.qj[i] == instr.dest){
								fpRS.vj[i] = instr.result;
								break;
							}
							if(fpRS.qk[i] == instr.dest){
								fpRS.vk[i] = instr.result;
								break;
							}
						}
						dataBus.cdbBuffer.pop();
						instr.state = commit;
						output.tDiag[instr.programLine][4] = numCycles;
					}
					break;
				default:
				break;
			}
}

void Commit(Instruction& inst, ReorderBuffer& rob, ReservationStation& intRS, ReservationStation& fpRS, int numCycles, timingDiagram output){

}
