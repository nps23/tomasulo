#pragma once
#include "Instruction.h"
#include "structures/functional_units.h"

class storeQueue {
public:
	deque<Instruction*> slQueue;
	int maxEntries;
	int cyclesToAddress;
	int cyclesToMem;
	int internalCycles;
	bool occupied;
	

	storeQueue(int mEntries, int addCycles, int memCycles) : addUnit(addCycles) {
		maxEntries = mEntries;
		cyclesToMem = memCycles;
		internalCycles = 0;
	}
	
	AddFunctinalUnit addUnit;

	int insert(Instruction* inst) {
		if (slQueue.size() < maxEntries) {
			slQueue.push_back(inst);
			maxEntries--;
			int index = table.size() - 1;
			return index;
		}
		return -2;
	}
	bool isFull() {
		if (maxEntries == slQueue.size()) {
			return true;
		}
		else {
			return false;
		}
	}
	int forwardFromStore(int queueIndex) {
		for (int i = queueIndex; i < slQueue.size(); i++) {
			if (slQueue[i]->address == slQueue[queueIndex]->address) {
				if (slQueue[i]->op_code == sd) {
					// Return false to show that forwarding will not happen as another stores goes to the same mem address.
					return -1;
				}
				else if (slQueue[i]->op_code == ld) {
					// Forward the value to the load and let the pipeline know that forwarding is occuring
					return true;
				}
			}
		}
		// If there are no mem instructions to the same address, no forwarding should occur. 
		return -1;
	}
};