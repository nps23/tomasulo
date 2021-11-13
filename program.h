#pragma once

#include "structures/reorder_buffer.h"

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
	int f_ls_register_operand;

	double result;

	int programLine;

	// ROB fields
	int robEntry;
	int instType;
	std::string destValue;

	// RS fields
	int rsEntry;
	std::string robIndex;
	double vj;
	double vk;
	std::string qj;
	std::string qk;
	bool busy;

	// Load store queue
	std::string operands[2];
	std::string dest;

	int pipelineTimer;

	int issue_start_cycle;
	int issue_end_cycle;
	int ex_start_cycle;
	int ex_end_cycle;
	int writeback_start_cycle;
	int writeback_end_cycle;

	// Pipeline steps
	void Issue(const ReorderBuffer& rob, const ReservationStation& rs);
	void Ex();
	void WriteBack();
	void Mem();
	void Commit();
};

void Issue()
{

}

void Ex()
{

}

void WriteBack()
{

}

void Commit()
{

}