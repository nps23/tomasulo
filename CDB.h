#include <queue>
#include <string>

using namespace std;

class CDBEntry {
	public:
		double value;
		int programLine;
		string destination;
		bool operator<(const CDBEntry& rightOp) const {
			return value < rightOp.value;
		}
};

class CDB {
	public:
		// Stored items are as follows: program line, value, destination
		priority_queue<CDBEntry, vector<CDBEntry>, greater<CDBEntry>> cdbBuffer;
};
