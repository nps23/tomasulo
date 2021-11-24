#pragma once

#include "../instruction.h"
#include <vector>
#include <utility>

class BranchPredictor
{
public:
	// Branch predictor will be configured to have 8 entries
	const int TABLE_SIZE = 8;
	std::vector<std::pair<bool, Instruction*>> table;
	BranchPredictor();
};

