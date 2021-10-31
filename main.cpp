#include <iostream>
#include <map>
#include "cpu_config.h"
#include "input_parser.h"





int main()
{
	std::cout << "Hello world! " << std::endl;
	CPUConfig config = ParseInput(input_file);
	PrintCPUConfig(config);
}