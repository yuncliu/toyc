#include "ast.h"

bool IsAllocaInst(Value* v) {
    return v->getValueID() == Value::InstructionVal + Instruction::Alloca;
}

bool IsGlobalVariable(Value* v) {
    return v->getValueID() == Value::GlobalVariableVal;
}
