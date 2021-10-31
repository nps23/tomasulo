#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "cpu_config.h"

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
			std::cout << line << std::endl;
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

		else if (line == "program")
			continue;
		// clear the eof flag
		// another function that lets you start at beginning of file, seekG

	}

	return config;	
}