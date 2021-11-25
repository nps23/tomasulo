#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "cpu_config.h"
#include "instruction.h"

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

		else if (line == "r_registers")
		{
			int num_lines;
			infile >> num_lines;
			for (int i = 0; i < num_lines; i++)
			{
				int address, value;
				infile >> address >> value;
				config.r_register_map[address] = value;
			}
		}

		else if (line == "f_registers")
		{
			int num_lines;
			infile >> num_lines;
			for (int i = 0; i < num_lines; i++)
			{
				int address;
				float value;
				infile >> address >> value;
				config.f_register_map[address] = value;
			}
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
					std::cout << "Pushing instruction: " << inst.op_code 
						<<" into the config" << std::endl;
					config.program.push_back(inst);
				}

				else if (opcode == "ld") {
					inst.op_code = ld;
					char fpId, rId, paran;
					int dest, operand, offset;
					infile >> fpId >> dest >> offset >> paran >> rId >> operand >> paran;

					inst.dest = dest;
					inst.f_ls_register_operand = operand;
					inst.offset = offset;
					std::cout << "Pushing instruction: " << inst.op_code
						<< " into the config" << std::endl;
					config.program.push_back(inst);
				}

				else if (opcode == "sd") {
					inst.op_code = sd;
					char fpId, rId, paran;
					int dest, operand, offset;
					infile >> fpId >> dest >> offset >> paran >> rId >> operand >> paran;

					inst.dest = dest;
					inst.f_ls_register_operand = operand;
					inst.offset = offset;
					std::cout << "Pushing instruction: " << inst.op_code
						<< " into the config" << std::endl;
					config.program.push_back(inst);
				}

				else if (opcode == "beq") {
					inst.op_code = beq;
					char reg_id;
					int l_operand, r_operand;
					int offset;
					infile >> reg_id >> l_operand >> reg_id >> r_operand;
					infile >> offset;
					inst.r_left_operand = l_operand;
					inst.r_right_operand = r_operand;
					inst.offset = offset;
					std::cout << "Pushing instruction: " << inst.op_code
						<< " into the config" << std::endl;
					config.program.push_back(inst);
				}

				else if (opcode == "bne") {
					inst.op_code = bne;
					char r;
					int l_operand, r_operand;
					int offset;
					infile >> r >> l_operand >> r >> r_operand;
					infile >> offset;
					inst.r_left_operand = l_operand;
					inst.r_right_operand = r_operand;
					inst.offset = offset;
					std::cout << "Pushing instruction: " << inst.op_code
						<< " into the config" << std::endl;
					config.program.push_back(inst);
				}

				else if (opcode == "add") {
					inst.op_code = add;
					char r;
					int dest, l_operand, r_operand;
					infile >> r >> dest >> r >> l_operand >> r >> r_operand;
					inst.dest = dest;
					inst.r_right_operand = r_operand;
					inst.r_left_operand = l_operand;
					std::cout << "Pushing instruction: " << inst.op_code
						<< " into the config" << std::endl;
					config.program.push_back(inst);
				}

				else if (opcode == "add_d") {
					inst.op_code = add_d;
					char f;
					int dest, l_operand, r_operand;
					infile >> f >> dest >> f >> l_operand >> f >> r_operand;
					inst.dest = dest;
					inst.f_right_operand = r_operand;
					inst.f_left_operand = l_operand;
					std::cout << "Pushing instruction: " << inst.op_code
						<< " into the config" << std::endl;
					config.program.push_back(inst);
				}

				else if (opcode == "add_i") {
					inst.op_code = add_i;
					char r;
					int l_operand, dest, immediate;
					infile >> r >> dest >> r >> l_operand >> immediate;
					inst.dest = dest;
					inst.r_left_operand = l_operand;
					inst.immediate = immediate;
					std::cout << "Pushing instruction: " << inst.op_code
						<< " into the config" << std::endl;;
					config.program.push_back(inst);
				}

				else if (opcode == "sub") {
					inst.op_code = sub;
					char r;
					int dest, l_operand, r_operand;
					infile >> r >> dest >> r >> l_operand >> r >> r_operand;
					inst.dest = dest;
					inst.r_right_operand = r_operand;
					inst.r_left_operand = l_operand;
					std::cout << "Pushing instruction: " << inst.op_code
						<< " into the config" << std::endl;
					config.program.push_back(inst);
				}

				else if (opcode == "sub_d") {
					inst.op_code = sub_d;
					char f;
					int dest, l_operand, r_operand;
					infile >> f >> dest >> f >> l_operand >> f >> r_operand;
					inst.dest = dest;
					inst.f_right_operand = r_operand;
					inst.f_left_operand = l_operand;
					std::cout << "Pushing instruction: " << inst.op_code
						<< " into the config" << std::endl;
					config.program.push_back(inst);
				}

				// there is an implicit assumption here that the dest will alwayys be able to hold the result
				else if (opcode == "mult_d") {
					inst.op_code = mult_d;
					char f;
					int dest, l_operand, r_operand;
					infile >> f >> dest >> f >> l_operand >> f >> r_operand;
					inst.dest = dest;
					inst.f_right_operand = r_operand;
					inst.f_left_operand = l_operand;
					std::cout << "Pushing back new instruction onto config" << std::endl;
					config.program.push_back(inst);
				}

				else if (opcode == "fin") {
					inst.op_code = fin;
					inst.program_end = true;
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

		else 
		{
			// local FU buffers requesting the central data bus
			continue;
		}
		
		if(infile.eof()){
			std::cout << "end of input file reached" << std::endl;
			break;
		}
	}
	std::cout << "Finished parsing input file" << std::endl;
	
	// initalize the BTB entries for the program
	for (int i = 0; i < config.program.size(); i++)
	{
		config.program[i].btb_index = i;
	}
	return config;	
}
