#pragma once

#include <queue>
#include <string>
#include <deque>
#include <algorithm>

#include "cpu_config.h"
#include "instruction.h"

using namespace std;

class intReg {
	public:
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
				for(int i = 0; i < 64; i++){
					mainMemory[i] = 0;
				}
				
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
	std::map<int, std::string> r_table;
	std::map<int, std::string> f_table;
	RAT() {
		for (int i = 0; i < 32; i++) {
			r_table[i] = "r" + std::to_string(i);
		}
		for (int i = 0; i < 32; i++) {
			f_table[i] = "f" + std::to_string(i);
		}
	}
};

class instructionBuffer {
	public:
		vector<Instruction*> inst;
		int curInst;
		instructionBuffer(){
			inst.resize(0);
			curInst = 0;
		}

		void update(Instruction* instruction)
		{
			inst.push_back(instruction);
		}
		
		void clear(Instruction* head)
		{
			if (inst.size() == 0)
			{
				std::cout << "Empty instruction buffer. continuing..." << std::endl;
				return;
			}
			inst.erase(std::remove(inst.begin(), inst.end(), head));
		}
		
		// Get the number of isntructions in the buffer for use in determining when the program is done running.
		int getNumInsts(){
			return inst.size();
		}
};

class ROB {
	public:
		int entry;
		int* instType;
		string* destValue;
		double* valueField;
		bool* readyField;
		int currentInst;
		int nextInsertedInst;
		int maxEntries;
		
		ROB(const CPUConfig& config){
			maxEntries = config.rob_entries;
			
			instType = new int[maxEntries];
			destValue = new string[maxEntries];
			valueField = new double[maxEntries];
			readyField = new bool[maxEntries];
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
		void insertOp(int opCodeInput, double vjInput = 0, double vkInput = 0, string qjInput = "", string qkInput = ""){
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

// TODO refactor this to be a vector
class timingDiagram {
	public:
		int (*tDiag)[11];
		int numLines;
		timingDiagram(int m){
			tDiag = new int[m][11];
			numLines = m;
			// Set the first column of the output 
			for(int i = 0; i < numLines; i++){
				tDiag[i][0] = i;
			}
			for(int i = 0; i < numLines; i++){
				tDiag[i][1] = -1;
				tDiag[i][2] = -1;
				tDiag[i][3] = -1;
				tDiag[i][4] = -1;
				tDiag[i][5] = -1;
				tDiag[i][6] = -1;
				tDiag[i][7] = -1;
				tDiag[i][8] = -1;
				tDiag[i][9] = -1;
				tDiag[i][10] = -1;
			}
		}
};

class ROM
{	
	public:
		std::vector<Instruction> program;
		Instruction* pc;
		ROM(deque<Instruction> iBuff){
			for(unsigned int i = 0; i < iBuff.size(); i++){
				program.push_back(iBuff[i]);
			}
			if (iBuff.size() != 0)
			{
				pc = &program[0];
			}
			//Instruction endDelimiter;
			//endDelimiter.instructionId = -1;
			//program.push_back(endDelimiter);
			
			//pc = &program[0];
		}
};
