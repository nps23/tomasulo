#pragma once

#include <iostream>
#include <map>
#include <vector>


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
	int cdb_buffer_entries; // will this change?

	
	
	// number of registers
	int num_r_registers;
	int num_f_registers;

	// registers
	std::vector<int> r_registers;
	std::vector<float> f_registers;

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

	std::cout << "Rob entries: " << config.rob_entries << std::endl;
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
}