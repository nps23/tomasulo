#include <iostream>
#include <map>
#include "cpu_config.h"
#include "input_parser.h"



int main()
{
	CPUConfig config = ParseInput(input_file);
	PrintCPUConfig(config);
}