#pragma once

#include "instruction.h"

bool Issue(Instruction& instr);
bool Ex(Instruction& instr);
bool WriteBack(Instruction& instr);
bool Commit(Instruction& instr);