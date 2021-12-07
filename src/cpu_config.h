#pragma once

#include <iostream>
#include <map>
#include <deque>
#include <vector>
#include "instruction.h"

// stores the configuration of the CPU
struct CPUConfig {
	/*
	For now:
		First value: is the #rs
		Second is cycles 
		Third is the number of FUs
	*/
	std::vector<int> fu_integer_adder;
	std::vector<int> fu_fp_adder;
	std::vector<int> fu_fp_mult;
	std::vector<int> fu_load_store;
	int cycles_mem;
	
	// additional configuration options
	int rob_entries;
	int cdb_buffer_entries; 

	// registers
	std::vector<int> r_registers;
	std::vector<float> f_registers;

	std::map<int, int> r_register_map;
	std::map<int, double> f_register_map;

	std::deque<Instruction> program;
	std::map<int, float> memory;

};