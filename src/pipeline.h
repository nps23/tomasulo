#pragma once

#include "instruction.h"

bool IssueFetch(Instruction* instr);
bool IssueDecode(Instruction* instr);
bool Ex(Instruction* instr);
bool Mem(Instruction* instr);
bool WriteBack(Instruction* instr);
bool Commit(Instruction* instr);
void InitializeInstruction(Instruction* instr);
Instruction* copyInstruction(const Instruction* right);
