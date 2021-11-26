#pragma once

#include "../cpu_config.h"

struct IntRegisterAliasingTableEntry
{
	bool is_mapped;
	int value;
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
	double value;
};

class FPRegisterAliasingTable
{
public:
	FPRegisterAliasingTableEntry table[32];
	FPRegisterAliasingTable (const CPUConfig& config);
};