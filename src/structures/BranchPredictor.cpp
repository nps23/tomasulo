#include "BranchPredictor.h"

#include <vector>
#include <utility>

BranchPredictor::BranchPredictor()
{
	table.resize(0);
	for (int i = 0; i < TABLE_SIZE; i++ )
	{
		table.push_back(std::make_pair(false, nullptr));
	}
}