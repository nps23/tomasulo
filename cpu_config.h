#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <deque>
#include "program.h"

// stores the configuration of the CPU
struct CPUConfig {
	// integer adder
	// we clean this up and wrap it into a map later. For now, first value
	// is the #rs, second is cycles, and third is the number of FUs
	std::vector<int> fu_integer_adder;
	std::vector<int> fu_fp_adder;
	std::vector<int> fu_fp_mult;
	std::vector<int> fu_load_store;
	
	// additional configuration options
	int rob_entries;
	int cdb_buffer_entries; 

	// number of registers
	int num_r_registers;
	int num_f_registers;

	// registers
	std::vector<int> r_registers;
	std::vector<float> f_registers;

	std::map<std::string, int> r_register_map;
	std::map<std::string, float> f_register_map;

	std::deque<Instruction> program;
	std::map<int, int> memory;

};

// print the status of the CPU for debug
void PrintCPUConfig(const CPUConfig& config)
{
	std::cout << "Integer adder: \n";
	std::cout << "\tnumber of reservation stations: ";
	std::cout << config.fu_integer_adder[0];
	std::cout << "\n \tcycles in ex: ";
	std::cout << config.fu_integer_adder[1];
	std::cout << "\n \tnumber of function units: ";
	std::cout << config.fu_integer_adder[2];
	std::cout << std::endl;

	std::cout << "Floating point adder: \n";
	std::cout << "\tnumber of reservation stations: ";
	std::cout << config.fu_fp_adder[0];
	std::cout << "\n \tcycles in ex: ";
	std::cout << config.fu_fp_adder[1];
	std::cout << "\n \tnumber of function units: ";
	std::cout << config.fu_fp_adder[2];
	std::cout << std::endl;

	std::cout << "Floating point multiplier: \n";
	std::cout << "\tnumber of reservation stations: ";
	std::cout << config.fu_fp_mult[0];
	std::cout << "\n \tcycles in ex: ";
	std::cout << config.fu_fp_mult[1];
	std::cout << "\n \tnumber of function units: ";
	std::cout << config.fu_fp_mult[2];
	std::cout << std::endl;

	std::cout << "Load store unit: \n";
	std::cout << "\tnumber of reservation stations: ";
	std::cout << config.fu_load_store[0];
	std::cout << "\n \tcycles in ex: ";
	std::cout << config.fu_load_store[1];
	std::cout << "\n \tnumber of function units: ";
	std::cout << config.fu_load_store[2];
	std::cout << std::endl;

	std::cout << "ROB entries: " << config.rob_entries << std::endl;
	std::cout << "CDB buffer entires: " << config.cdb_buffer_entries << std::endl;

	std::cout << "Number r registers: " << config.num_r_registers;
	for (const auto& value : config.r_registers)
	{
		std::cout << "\n \t" << value;
	}
	std::cout << std::endl;

	std::cout << "Number f registers: " << config.num_f_registers;
	for (const auto& value : config.f_registers)
	{
		std::cout << "\n \t" << value;
	}
	std::cout << std::endl;

	std::cout << "Inital r register states:";
	for (const auto& elem : config.r_register_map)
	{
		std::cout << "\n \t" <<  elem.first << " " << elem.second;
	}
	std::cout << std::endl;

	std::cout << "Inital f register states:";
	for (const auto& elem : config.f_register_map)
	{
		std::cout << "\n \t" << elem.first << " " << elem.second;
	}
	std::cout << std::endl;

	std::cout << "Program:" << std::endl;
	for (const auto& instruction : config.program)
	{
		switch (instruction.op_code)
		{
		case 0:
			std::cout << "NOP" << std::endl;
			break;
		case 1:
			std::cout << "ld_fa" << std::endl;
			break;
		case 2:
			std::cout << "sd_fa" << std::endl;
			break;
		case 3:
			std::cout << "beq" << std::endl;
			break;
		case 4:
			std::cout << "bne" << std::endl;
			break;
		case 5:
			std::cout << "add" << std::endl;
			break;
		case 6:
			std::cout << "add_d" << std::endl;
			break;
		case 7:
			std::cout << "add_i" << std::endl;
			break;
		case 8:
			std::cout << "sub" << std::endl;
			break;
		case 9:
			std::cout << "sub_d" << std::endl;
			break;
		case 10:
			std::cout << "mult_d" << std::endl;
			break;
		default:
			std::cout << "Bad instruction. Something went wrong" << std::endl;
			break;
		}
	}

	std::cout << "Memory" << std::endl;
	for (const auto& address : config.memory)
	{
		std::cout << "Address " << address.first << ": " << address.second << std::endl;
	}
}
