#include "BranchPredictor.h"

#include <vector>
#include <utility>

BranchPredictor::BranchPredictor()
{
	table.resize(TABLE_SIZE);
	for (auto& value : table)
	{
		table.push_back(std::make_pair(false, nullptr));
	}
}