#include <queue>
#include <string>
#include "cpu_config.h"
#include "program.h"

using namespace std;

class intReg {
	public:
		// TODO decide if we want to just change this to be a map anyways
		int intRegFile[32];
		intReg(const CPUConfig& config) {
			intRegFile[0] = 0;
			for (const auto& value : config.r_register_map)
			{
				intRegFile[value.first] = value.second;
			}
		}
};

class fpReg {
	public:
		double fpRegFile[32];
		fpReg(const CPUConfig& config) {
			for (const auto& value : config.f_register_map)
			{
				fpRegFile[value.first] = value.second;
			}
		}
};

class cpuMemory {
	public:
		double mainMemory[64];
		cpuMemory(const CPUConfig& config) {
			if (!config.memory.empty())
			{
				for (const auto& address : config.memory)
				{
					mainMemory[address.first] = address.second;
				}
			}
		}
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
		vector<Instruction> inst;
		int curInst;
		instructionBuffer(){
			curInst = 0;
		}
		vector<Instruction> operator=(const instructionBuffer& rhs){
			// First reset the buffer
			vector<Instruction> instNew;
			
			// Fill the buffer with the right hand operand
			for(unsigned int i = 0; i < rhs.inst.size(); i++){
				instNew.push_back(rhs.inst[i]);
			}
			return instNew;
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
					break;
				}
			}
			return isFull;
		}
};

class RS {
	public:
		int* opCode;
		string* robIndex;
		double* vj;
		double* vk;
		string* qj;
		string* qk;
		bool* busy;
		int maxRS;
		RS(int rsSize){
			opCode = new int[rsSize];
			robIndex = new string[rsSize];
			vj = new double[rsSize];
			vk = new double[rsSize];
			qj = new string[rsSize];
			qk = new string[rsSize];
			busy = new bool[rsSize];
			maxRS = rsSize; 
			for(int i = 0; i < rsSize; i++){
				opCode[i] = -1;
				robIndex[i] = "NULL";
				vj[i] = 0.0;
				vk[i] = 0.0;
				qj[i] = "NULL";
				qk[i] = "NULL";
				busy[i] = false;
			}
		}
		void insertOp(int opCodeInput, double vjInput, double vkInput, string qjInput, string qkInput){
			int rsIndex;
			// Find an open spot in the RS
			for(int i = 0; i < maxRS; i++){
				if(busy[i] == true){
					rsIndex = i;
					break;
				}
			}
			opCode[rsIndex] = opCodeInput;
			vj[rsIndex] = vjInput;
			vk[rsIndex] = vkInput;
			qj[rsIndex] = qjInput;
			qk[rsIndex] = qkInput;
			busy[rsIndex] = true;
		}
		bool checkFull(){
			bool isFull = true;
			for(int i = 0; i < maxRS; i++){
				if(busy[i] == false){
					isFull = false;
					break;
				}
			}
			return isFull;
		}
};

class timingDiagram {
	public:
		int (*tDiag)[6];
		timingDiagram(int m){
			tDiag = new int[m][6];
		}
};
