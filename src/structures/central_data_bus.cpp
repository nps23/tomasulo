#include <algorithm>
#include <vector>
#include <iostream>

#include "central_data_bus.h"

// Assume unlimited buffer space. Another FU will be able to operate before issuing next instruciton
cdb::cdb()
{
	occupied = false;
}

bool cdb::isEmpty()
{
	return buffer.empty();
}
// make sure to fill the buffer before calling this method
void cdb::clear(Instruction* instr)
{
	if (buffer.size() == 0)
	{
		throw std::runtime_error("Trying to remove element from an empty CDB");
	}
	buffer.erase(std::remove(buffer.begin(), buffer.end(), instr), buffer.end());
}

void cdb::insert(Instruction* instr)
{
	buffer.push_back(instr);
}