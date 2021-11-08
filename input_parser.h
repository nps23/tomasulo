#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "cpu_config.h"
#include "program.h"

std::string input_file = "test/input.txt";

//parse an input file and return a CPU config struct
CPUConfig ParseInput(std::string& input_file)
{
	CPUConfig config;
	std::string line;
	std::ifstream infile(input_file);
	
	while (!infile.eof())
	{
		infile >> line;
		// extract functional unit config options 
		if (line[0] == 'f' && line[1] == 'u')
		{
			int rs, cycles_ex, num_fus;
			infile >> rs >> cycles_ex >> num_fus;
			if (line == "fu_integer_adder")
			{
				config.fu_integer_adder.push_back(rs);
				config.fu_integer_adder.push_back(cycles_ex);
				config.fu_integer_adder.push_back(num_fus);
			}
			else if (line == "fu_fp_adder")
			{
				config.fu_fp_adder.push_back(rs);
				config.fu_fp_adder.push_back(cycles_ex);
				config.fu_fp_adder.push_back(num_fus);
			}
			else if (line == "fu_fp_mult")
			{
				config.fu_fp_mult.push_back(rs);
				config.fu_fp_mult.push_back(cycles_ex);
				config.fu_fp_mult.push_back(num_fus);
			}
			else if (line == "fu_load_store")
			{
				config.fu_load_store.push_back(rs);
				config.fu_load_store.push_back(cycles_ex);
				config.fu_load_store.push_back(num_fus);
			}
		}

		else if (line == "rob_entries")
		{
			int rob_entries;
			infile >> rob_entries;
			config.rob_entries = rob_entries;
		}

		else if (line == "cdb_buffer_entries")
		{
			int cdb_buffer_entires;
			infile >> cdb_buffer_entires;
			config.cdb_buffer_entries = cdb_buffer_entires;
		}

		else if (line == "num_r_registers")
		{
			int number_registers;
			infile >> number_registers;
			config.num_r_registers = number_registers;
			for (int i = 0; i < number_registers; i++)
			{
				int value;
				infile >> value;
				config.r_registers.push_back(value);
			}
		}

		else if (line == "num_f_registers")
		{
			int number_registers;
			infile >> number_registers;
			config.num_f_registers = number_registers;
			for (int i = 0; i < number_registers; i++)
			{
				int value;
				infile >> value;
				config.f_registers.push_back(value);
			}
		}

		else if (line[0] == 'r')
		{
			int value;
			infile >> value;
			config.r_register_map[line] = value;
		}

		else if (line[0] == 'f')
		{
			float value;
			infile >> value;
			config.f_register_map[line] = value;
		}


		// better ways to parse this for sure, but I'd rather not create another new map
		else if (line == "program") {
			int num_prog_lines;
			infile >> num_prog_lines;
			for (int i = 0; i < num_prog_lines; i++) {
				Instruction inst;
				std::string opcode;
				infile >> opcode;
				
				if (opcode == "nop")
				{
					inst.op_code = nop;
					config.program.push_back(inst);
				}

				else if (opcode == "ld_fa") {
					inst.op_code = ld_fa;
					std::string dest, op1;
					infile >> dest >> op1;
					inst.dest = dest;
					inst.operands[0] = op1;
					config.program.push_back(inst);
				}

				else if (opcode == "sd_fa") {
					inst.op_code = sd_fa;
					std::string dest, op1;
					infile >> dest >> op1;
					inst.dest = dest;
					inst.operands[1] = op1;
					config.program.push_back(inst);
				}

				// rest of the instruction
				else if (opcode == "beq") {
					inst.op_code = beq;
					std::string op1, op2, dest;
					infile >> op1 >> op2 >> dest;
					inst.dest = dest;
					inst.operands[0] = op1;
					inst.operands[1] = op2;
					config.program.push_back(inst);
				}
				else if (opcode == "bne") {
					inst.op_code = bne;
					std::string op1, op2, dest;
					infile >> op1 >> op2 >> dest;
					inst.dest = dest;
					inst.operands[0] = op1;
					inst.operands[1] = op2;
					config.program.push_back(inst);
				}

				else if (opcode == "add") {
					inst.op_code = add;
					std::string dest, op1, op2;
					infile >> dest >> op1 >> op2;
					inst.dest = dest;
					inst.operands[0] = op1;
					inst.operands[1] = op2;
					config.program.push_back(inst);
				}

				else if (opcode == "add_d") {
					inst.op_code = add_d;
					std::string dest, op1, op2;
					infile >> dest >> op1 >> op2;
					inst.dest = dest;
					inst.operands[0] = op1;
					inst.operands[1] = op2;
					config.program.push_back(inst);
				}

				else if (opcode == "add_i") {
					inst.op_code = add_i;
					std::string dest, op1, op2;
					infile >> dest >> op1 >> op2;
					inst.dest = dest;
					inst.operands[0] = op1;
					inst.operands[1] = op2;
					config.program.push_back(inst);
				}

				else if (opcode == "sub") {
					inst.op_code = sub;
					std::string dest, op1, op2;
					infile >> dest >> op1 >> op2;
					inst.dest = dest;
					inst.operands[0] = op1;
					inst.operands[1] = op2;
					config.program.push_back(inst);
				}

				else if (opcode == "sub_d") {
					inst.op_code = sub_d;
					std::string dest, op1, op2;
					infile >> dest >> op1 >> op2;
					inst.dest = dest;
					inst.operands[0] = op1;
					inst.operands[1] = op2;
					config.program.push_back(inst);
				}

				else if (opcode == "mult_d") {
					inst.op_code = mult_d;
					std::string dest, op1, op2;
					infile >> dest >> op1 >> op2;
					inst.dest = dest;
					inst.operands[0] = op1;
					inst.operands[1] = op2;
					config.program.push_back(inst);
				}
			}
		}

		else if (line == "memory") {
			int num_cpu_lines;
			infile >> num_cpu_lines;
			for (int i = 0; i < num_cpu_lines; i++)
			{
				int address, value;
				infile >> address >> value;
				config.memory[address] = value;
			}
		}
	}
	return config;	
}
