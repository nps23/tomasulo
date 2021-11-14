#pragma once

#include "instruction.h"

bool Issue(Instruction& instr);
bool Ex();
bool WriteBack();
bool Commit();