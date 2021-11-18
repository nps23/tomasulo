#include <algorithm>
#include <vector>

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
		throw "Error! Trying to remove from an empty CDB buffer!";
	}
	buffer.erase(std::remove(buffer.begin(), buffer.end(), instr));
}

void cdb::insert(Instruction* instr)
{
	buffer.push_back(instr);
}