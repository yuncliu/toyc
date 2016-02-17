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

class StmtlistAST;

class ExprAST {
    public:
    ExprAST() {
    }
    virtual ~ExprAST() {
    }
    virtual Value* codegen(StmtlistAST* block) {
        return NULL;
    }

    virtual Value* codegen() {
        return NULL;
    }
};

class IdExprAST:public ExprAST{
    public:
    string      name;
    string      type;
    IdExprAST() {
    }
    IdExprAST(string s) :name(s){
    }
    virtual ~IdExprAST() {
    }
    virtual Value* codegen(StmtlistAST* block);
};

class IntExprAST: ExprAST {
    public:
    int value;
    IntExprAST(int i):value(i) {
    }
    ~IntExprAST() {
    }
    Value* codegen() {
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value, true);
    }
};

class VarExprAST: ExprAST {
    public:
        string type;
        string name;
        VarExprAST(string t, string n)
            :type(t), name(n)
        { }
        ~VarExprAST() {
        }
        Value* codegen() {
            return NULL;
        }
};


class BinaryExprAST {
    public:
    char op;
    ExprAST* left;
    ExprAST* right;
    BinaryExprAST(ExprAST* l, ExprAST* r):left(l), right(r) {
    }
    virtual ~BinaryExprAST() {
    }
    virtual Value* codegen() {
        Value* l = left->codegen();
        Value* r = right->codegen();
        return Single::getBuilder()->CreateAdd(l, r);
    }
};

class FuncAST;

class StmtAST {
    public:
        ExprAST* value;
        StmtAST():value(NULL) {
        }
        StmtAST(ExprAST* e):value(e) {
        }
        ~StmtAST() {
        }
        virtual void codegen(StmtlistAST* block) {
            if (NULL != value) {
                value->codegen();
            }
            Value *retval = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 10, true);
            Single::getBuilder()->CreateRet(retval);
        }
};

class ReturnStmtAST: public StmtAST {
    public:
        ExprAST* expr;
        ReturnStmtAST(ExprAST* e): expr(e) {
        }
        ~ReturnStmtAST(){
        }
        //void codegen() {
        virtual void codegen(StmtlistAST* block) {
            printf("codegen of Return Stmt\n");
            Value *retval = expr->codegen(block);
            Single::getBuilder()->CreateRet(retval);
        }
};

class StmtlistAST {
    public:
        vector<StmtAST*> stmts;
        map<string, Value*> locals;
        StmtlistAST() {
        }
        ~StmtlistAST(){
        }
        virtual void codegen() {
            if (stmts.size() == 0 ) {
                printf("no statements\n");
            } else {
                printf("%ld statements\n", stmts.size());
            }
            for (auto i:stmts) {
                i->codegen(this);
            }
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
            // Set names for all arguments.
            unsigned Idx = 0;
            for (Function::arg_iterator AI = F->arg_begin();
                    Idx != functype->arg_list->names.size();
                    ++AI, ++Idx) {
                AI->setName(functype->arg_list->names[Idx]);
                body->locals.insert(pair<string, Value*>(functype->arg_list->names[Idx], AI));
            }
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
