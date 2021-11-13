#pragma once

#include <queue>
#include <string>

using namespace std;

class CDBEntry {
	public:
		double value;
		int programLine;
		string dest;
};
class arbitrate {
	public:
		bool operator()(const CDBEntry& rightOp, const CDBEntry& leftOp) const {
			return leftOp.programLine < rightOp.programLine;
		}
}; 
class CDB {
	public:
		// Stored items are as follows: program line, value, destination
		priority_queue<CDBEntry, vector<CDBEntry>, arbitrate> cdbBuffer;
};
