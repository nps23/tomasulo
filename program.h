#pragma once

#include <queue>
#include <string>

enum OpCode {
		nop = 0,
		ld_fa,
		sd_fa,
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
		abort
	};
	
struct Instruction {
	// metadata associatd with the instruction itself
	OpCode op_code;
	

	PipelineState state; 
	int programLine;
	
	// ROB fields
	int robEntry;
	int instType;
	std::string destValue;

	// Load store queue
	std::string operands[2];
	std::string dest;


	
	int pipelineTimer;
	double result;
};
