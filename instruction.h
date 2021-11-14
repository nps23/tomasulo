#pragma once

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