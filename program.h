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
	
enum State {
		null = 0,
		issue,
		ex,
		mem, 
		wb,
		commit
	};
	
struct Instruction {
	OpCode op_code;
	State state; 
	
	// we will at most have three operands
	std::string operands[2];
	std::string dest;
	int pipelineTimer;
	
};
