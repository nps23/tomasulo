#pragma once

#include "instruction.h"

bool Issue(Instruction& instr);
void Ex();
void WriteBack();
void Commit();