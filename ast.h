/**
 *
 */
#ifndef _AST_H_
#define _AST_H_
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <string>
#include <vector>
#include "Visitor.h"
#define  LOG(fmt, args...)  if (0) printf(fmt, ##args)
using namespace llvm;

class Single {
    static Module*      m;
    static IRBuilder<>* b;
    private:
    Single() {
    }
    public:
    static Module* getModule() {
        if (m == NULL) {
            m = new Module("main", getGlobalContext());
        }
        return m;
    }
    static IRBuilder<>* getBuilder() {
        if (b == NULL) {
            b = new IRBuilder<>(getGlobalContext());
        }
        return b;
    }
    static void ReSet() {
        m = NULL;
        b = NULL;
    }
    static std::map<std::string, Value*> globalNamedValue;
    static bool IsGlobal(std::string name) {
        return Single::globalNamedValue.find(name) != Single::globalNamedValue.end();
    }
};

bool IsAllocaInst(Value* v);
bool IsGlobalVariable(Value* v);

#endif
