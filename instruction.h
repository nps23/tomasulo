#pragma once

#include <string>

enum OpCode {
	nop = 0,
	ld,
	sd,
	beq,
	bne,
	add,   //issue,ex
	add_d, //issue,ex
	add_i,
	sub,   //issue,ex
	sub_d, //issue,ex
	mult_d, //issue,ex
	fin
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
	// metadata to track and reference the instruction
	int instructionId{ 0 };
	PipelineState state;
	bool end{ false };				// should be only true for fin instructions

	// INSTRUCTION METADATA
	OpCode op_code;
	// Integer ALU operands
	int r_right_operand{ -1 };
	int r_left_operand{ -1 };
	// FP ALU operands
	int f_right_operand{ -1 };
	int f_left_operand{ -1 };
	// Load store queue
	int offset{ -1 };
	int immediate{ -1 };
	int r_ls_register_operand{ -1 };
	int f_ls_register_operand{ -1 };
	// Shared instruction metadata
	int dest{ -1 };
	double result{ -1 };
	int programLine{ -1 };

	// ROB fields
	//int robEntry; this should be taken care of 
	int instType{ -1 };
	// int rs, rb, rt
	bool rob_busy{ false };

	// RS fields
	int rsEntry{ -1 };
	std::string robIndex;
	double vj{ -1 };
	double vk{ -1 };
	int qj{ -1 };  // instructions which will show 
	int qk{ -1 };
	bool rs_busy{ false };

	//CDB fields
	bool occupying_bus{ false };

	int pipelineTimer{ -1 };

	// Timing diagram values
	int issue_start_cycle{ -1 };
	int issue_end_cycle{ -1 };
	int ex_start_cycle{ -1 };
	int ex_end_cycle{ -1 };
	int mem_start_cycle{ -1 };
	int mem_end_cycle{ -1 };
	int writeback_start_cycle{ -1 };
	int writeback_end_cycle{ -1 };
	int commit_start_cycle{ -1 };
	int commit_end_cycle{ -1 };

	//PIPELINE FLAGS
	bool just_fetched = false;
	bool writeback_begin = true;
	bool commit_begin = true;
	bool ex_begin = true;
	bool issued = false;

	//BTB 
	int btb_index{ -1 };
	bool triggered_branch{ false };
	Instruction* sourceInstr{ nullptr };
	Instruction* realizedInstructionTargetTarget{ nullptr };
};
