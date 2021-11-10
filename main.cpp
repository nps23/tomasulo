// Main file for controlling the tomasulo algorithm project for ECE2162
// David Herr and Nathan Stodick
#include <iostream>
#include <map>
#include <string>
#include <queue>

#include "cpu_config.h"
#include "input_parser.h"
#include "data_holding_structures.h"
#include "CDB.h"
#include "program.h"
#include "simulate.h"

using namespace std;


//void programFSM(Instruction& instr);

int main()
{
	Simulate();
	return 0;
}

//void programFSM(Instruction& instr){
//	switch(instr.state){
//		case issue:
//			switch(instr.op_code){
//				case nop:
//					if(rob.checkFull() == false){
//						rob.insert(instr.op_code(nop, "NULL", 0));
//						instr.state = ex;
//						 INSERT INTO TIMING DIAGRAM
//					}
//				default:
//			}
//			break;
//		case ex:
//			switch(instr.op_code){
//				case nop:
//					instr.state = wb;
//					 INSERT INTO TIMING DIAGRAM
//				default:
//			}
//			break;
//		case mem:
//			switch(instr.op_code){
//				default:
//			}
//			break;
//		case wb:
//			switch(instr.op_code){
//				case nop:
//					instr.state = commit;
//					 INSERT INTO TIMING DIAGRAM
//				default:
//			}
//			break;
//		case commit:
//			switch(instr.op_code){
//				case nop:
//					rob.commit();
//					instr.state = null;
//					 INSERT INTO TIMING DIAGRAM
//				default:
//			}
//			break;
//		 Handles Null case
//		default:
//			 If the instruction state is null, ignore the instruction.
//	}
//}
