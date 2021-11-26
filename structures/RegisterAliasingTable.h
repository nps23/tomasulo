#pragma once

#include "../cpu_config.h"

struct IntRegisterAliasingTableEntry
{
	bool is_mapped;
	std::vector<int> map_entries;
	int register_value;
};

class IntRegisterAliasingTable
{
public:
	IntRegisterAliasingTableEntry table[32];
	IntRegisterAliasingTable(const CPUConfig& config);
};

struct FPRegisterAliasingTableEntry
{
	bool is_mapped;
	std::vector<int> map_entries;
	double register_value;
};

class FPRegisterAliasingTable
{
public:
	FPRegisterAliasingTableEntry table[32];
	FPRegisterAliasingTable (const CPUConfig& config);
};