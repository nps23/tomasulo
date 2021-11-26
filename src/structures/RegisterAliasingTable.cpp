
#include "RegisterAliasingTable.h"
#include "../cpu_config.h"

IntRegisterAliasingTable::IntRegisterAliasingTable(const CPUConfig& config)
{
	for (int i = 0; i < 32; i++)
	{
		table[i].is_mapped = false;
		table[i].value = 0;
	}

	for (const auto& elem : config.r_register_map)
	{
		table[elem.first].value = elem.second;
	}
}

FPRegisterAliasingTable::FPRegisterAliasingTable(const CPUConfig& config)
{
	for (int i = 0; i < 32; i++)
	{
		table[i].is_mapped = false;
		table[i].value = 0.0;
	}

	for (const auto& elem : config.f_register_map)
	{
		table[elem.first].value = elem.second;
	}
}