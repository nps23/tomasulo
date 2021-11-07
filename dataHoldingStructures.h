#include <queue>
#include <string>
#include "cpu_config.h"

using namespace std;

class intReg {
	public:
		// TODO decide if we want to just change this to be a map anyways
		int intRegFile[32];
		intReg(const CPUConfig& config) {
			intRegFile[0] = 0;
			for (const auto& value : config.r_register_map)
			{
				int index = (int)value.first[1];
				intRegFile[index] = value.second;
			}
		}
};

class fpReg {
	public:
		double fpRegFile[32];
		fpReg(const CPUConfig& config) {
			for (const auto& value : config.f_register_map)
			{
				int index = (int)value.first[1];
				fpRegFile[index] = value.second;
			}
		}
};

class cpuMemory {
	public:
		double mainMemory[64];
		cpuMemory() {}
		void inputMem(double input, int address) {
			mainMemory[address] = input;
		}
};

class storeQueue {
	public:
		queue<int> memAddress;
		queue<double> memValue;
		int addressCalc(int base, int offset){
				return base + offset;
		}
};

class RAT {
	public:
		string intMapping[32][2];
		string fpMapping[32][2];
		RAT(){
			for(int i = 0; i < 32; i++){
				intMapping[i][0] = "R" + to_string(i);
				intMapping[i][1] = "R" + to_string(i);
			}
			for(int i = 0; i < 32; i++){
				fpMapping[i][0] = "F" + to_string(i);
				fpMapping[i][1] = "F" + to_string(i);
			}
		}
};

class instructionBuffer {
	public:
		int instOpcode[100];
		string op1[100];
		string op2[100];
		string result[100];
		int curInst;
		instructionBuffer(){
			curInst = 0;
		}
};

class ROB {
	public:
		int* instType;
		string* destValue;
		double* valueField;
		bool* readyField;
		int currentInst;
		int nextInsertedInst;
		int maxEntries;
		
		ROB(const CPUConfig& config){
			int numEntries = config.rob_entries;
			
			instType = new int[numEntries];
			destValue = new string[numEntries];
			valueField = new double[numEntries];
			readyField = new bool[numEntries];
			currentInst = 0;
			nextInsertedInst = 0;
			for(int i = 0; i < maxEntries; i++){
				instType[i] = -1;
				destValue[i] = "NULL";
				valueField[i] = 0.0;
				readyField[i] = false;
			}
		}
		void insert(int opCode, string dest, double value){
			if(nextInsertedInst == 0){
				instType[0] = opCode;
				destValue[0] = dest;
				valueField[0] = value;
				readyField[0] = false;
				nextInsertedInst++;
			}else{
				instType[nextInsertedInst] = opCode;
				destValue[nextInsertedInst] = dest;
				valueField[nextInsertedInst] = value;
				readyField[nextInsertedInst] = false;
				if((nextInsertedInst + 1) == maxEntries){
					nextInsertedInst = 0;
				}else{
					nextInsertedInst++;
				}
			}
		}
		void commit(){
			if(readyField[currentInst] == true){
				instType[currentInst] = -1;
				destValue[currentInst] = "NULL";
				valueField[currentInst] = 0.0;
				readyField[currentInst] = false;
				currentInst++;
			}
		}
		bool checkFull(){
			bool isFull = true;
			for(int i = 0; i < maxEntries; i++){
				if(instType[i] == -1){
					isFull = false;
				}
			}
			return isFull;
		}
};
