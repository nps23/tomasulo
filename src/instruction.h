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
	mult_d,				
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

struct Instruction {
	
	// INSTRUCTION METADATA
	int instructionId{ 0 };
	PipelineState state;
	bool program_end{ false };			
	OpCode op_code;

	// Integer ALU operands
	int r_right_operand{ -1 };
	int r_left_operand{ -1 };
	// FP ALU operands
	int f_right_operand{ -1 };
	int f_left_operand{ -1 };
	// Load store queue and branch instructions
	int offset{ -1 };
	int immediate{ -1 };
	int r_ls_register_operand{ -1 };		
	int f_ls_register_operand{ -1 };
	// Shared instruction metadata
	double result{ -1 };
	int programLine{ -1 };

	// ROB fields
	int instType{ -1 };
	double value{ -1 };										// This will be the same field used for loads and stores as well
	bool rob_busy{ false };

	// RS fields
	double vj{ -1 };
	double vk{ -1 };
	int qj{ -1 };											// instructions which will show 
	int qk{ -1 };
	bool rs_busy{ false };
	int dest{ -1 };

	// BRANCH UNIT 
	int btb_index{ -1 };
	bool triggered_branch{ false };
	Instruction* source_instruction{ nullptr };				// point to the sourc instruction 
	Instruction* realized_instruction_target{ nullptr };	// resolved in the IssueDecode stage
	Instruction* btb_target_instruction{ nullptr };
	bool mispredict{ false };
	bool branch_false_positive{ false };
	bool branch_false_negative{ false };

	// LOAD STORE QUEUE
	int address{ -1 };
	bool recieving_forward{ false };

	//CDB fields
	bool occupying_bus{ false };

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

	// PIPELINE FLAGS
	bool just_fetched{ false };
	bool writeback_begin{ true };;
	bool commit_begin{ true };
	bool ex_begin{ true };
	bool issued{ false };

	// FUNCTIONAL UNIT FLAGS
	bool occupying_floating_point_add_unit{ false };
	int floating_point_add_unit_cycle{ -1 };
	bool occupying_floating_point_mult_unit{ false };
	int floating_point_mult_unit_cycle{ -1 };
	bool occuping_integer_add_unit{ false };
	int integer_add_unit_cycle{ -1 };
};
