
#include "RegisterAliasingTable.h"
#include "../cpu_config.h"

IntRegisterAliasingTable::IntRegisterAliasingTable(const CPUConfig& config)
{
	for (int i = 0; i < 32; i++)
	{
		table[i].is_mapped = false;
		table[i].register_value = 0;
	}

	for (const auto& elem : config.r_register_map)
	{
		table[elem.first].register_value = elem.second;
	}
}

// Remove RAT mappings that have been either commited or squashed
void IntRegisterAliasingTable::ClearRatId(int id)
{
	for (auto& entry : table)
	{
		for (auto& map : entry.map_entries)
		{
			if (map == id)
			{
				auto& map_ids = entry.map_entries;
				map_ids.erase(std::remove(map_ids.begin(), map_ids.end(), id));
				// we should only have a single RAT entry per unique id
				continue;
			}
		}
	}
}

FPRegisterAliasingTable::FPRegisterAliasingTable(const CPUConfig& config)
{
	for (int i = 0; i < 32; i++)
	{
		table[i].is_mapped = false;
		table[i].register_value = 0.0;
	}

	for (const auto& elem : config.f_register_map)
	{
		table[elem.first].register_value = elem.second;
	}
}

void FPRegisterAliasingTable::ClearRatId(int id)
{
	for (auto& entry : table)
	{
		for (auto& map : entry.map_entries)
		{
			if (map == id)
			{
				auto& map_ids = entry.map_entries;
				map_ids.erase(std::remove(map_ids.begin(), map_ids.end(), id));
				// we should only have a single RAT entry per unique id
				continue;
			}
		}
	}
}
