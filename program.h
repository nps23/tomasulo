#pragma once

#include <queue>
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
	int r_right_operand;
	int r_left_operand;

	int f_right_operand;
	int f_left_operand;

	// populate only for load/store
	int offset;
	int immediate;
	int r_ls_register_operand;
	int f_ls_register_operand;
	int dest;
	
	int pipelineTimer;

};
