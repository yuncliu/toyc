/**
 * int a = 0;
 * got a IdAST x
 * x.type   = "int"
 * x.id     = "a"
 * x.value  = 0
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
#define  LOG(fmt, args...)  if (0) printf(fmt, ##args)
using namespace std;
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
};

class AST {
        static Module* TheModule;
        static IRBuilder<>* Builder;
    public:
        AST() {
        }
        virtual ~AST() {
        }
        virtual Value* codegen() {
            printf("code gen from base class\n");
            return NULL;
        }
};

class ExprAST;

class IdAST:public AST{
    public:
    string      name;
    string      type;
    AST*        value;
    IdAST() {
    }
    IdAST(string s) :name(s){
    }
    virtual ~IdAST() {
    }
    virtual Value* codegen() {
        value->codegen();
        printf("id ast code gen\n");
        return 0;
    }
};

class ExprAST:public AST {
    public:
    char op;
    int value;
    ExprAST() {
    }
    ~ExprAST() {
    }
};

class VarAST {
    public:
        string type;
        string name;
        VarAST(string t, string n)
            :type(t), name(n)
        { }
        ~VarAST() {
        }
};

class FuncAST;

class StmtAST:public AST {
    public:
        AST* value;
        StmtAST():value(NULL) {
        }
        ~StmtAST() {
        }
        virtual Value* codegen() {
            if (NULL != value) {
                value->codegen();
            }
            Value *retval = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 10, true);
            Single::getBuilder()->CreateRet(retval);
            return 0;
        }
};

class StmtlistAST:public AST {
    public:
        vector<StmtAST*> stmts;
        StmtlistAST() {
        }
        ~StmtlistAST(){
        }
        virtual Value* codegen() {
            if (stmts.size() == 0 ) {
                printf("no statements\n");
            } else {
                printf("%ld statements\n", stmts.size());
            }
            for (auto i:stmts) {
                i->codegen();
            }
            return 0;
        }
};

class FuncArgsAST {
    public:
        std::vector<string> names;
        std::vector<Type*>  args;
        FuncArgsAST(){
        }
        ~FuncArgsAST() {
        }
        void addarg(string name, string type) {
            names.push_back(name);
            if (type == "int") {
                args.push_back(Type::getInt32Ty(getGlobalContext()));
            }
        }
        std::vector<string>  getNames() {
            return names;
        }
        std::vector<Type*>  getArgs() {
            return args;
        }
};

class FuncTypeAST {
    public:
        FuncArgsAST* arg_list;
        FuncTypeAST() {
        }
        ~FuncTypeAST() {
        }
        FunctionType* codegen() {
            //std::vector<Type*> args;
            //args.push_back(Type::getInt32Ty(getGlobalContext()));
            //PointerType* x = Type::getInt8PtrTy(getGlobalContext());
            //PointerType* p = PointerType::get(x, 0);
            //args.push_back(p);
            FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()),
                    arg_list->getArgs(), false);
            return FT;
        }
};

class FuncAST {
    public:
        //FunctionType* functype;
        string       name;
        FuncTypeAST* functype;
        StmtlistAST* body;
        FuncAST(string n):name(n) {
            functype = new FuncTypeAST();
        }
        ~FuncAST() {
        }
        Function* codegen() {
            Function* F = Function::Create(functype->codegen(), Function::ExternalLinkage, name,
                    Single::getModule());
            BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
            Single::getBuilder()->SetInsertPoint(BB);
            if (NULL != body) {
                body->codegen();
            } else {
                printf("body is NULL\n");
            }
            return F;
        }
};


class ProgramAST:public AST {
    public:
};


#endif
