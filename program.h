#pragma once

#include <queue>
#include <string>


struct Instruction {
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
	} op_code;
	
	// we will at most have three operands
	std::string operands[2];
	std::string dest;
};