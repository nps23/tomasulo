#pragma once

#include <iostream>
#include <map>
#include <deque>
#include <vector>
#include "instruction.h"

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

	// registers
	std::vector<int> r_registers;
	std::vector<float> f_registers;

	std::map<int, int> r_register_map;
	std::map<int, double> f_register_map;

	std::deque<Instruction> program;
	std::map<int, int> memory;

};