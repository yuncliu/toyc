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
#include <string>
#include <vector>
#define  LOG(fmt, args...)  if (0) printf(fmt, ##args)
using namespace std;

class AST {
    public:
        AST() {
        }
        virtual ~AST() {
        }
        virtual int codegen() {
            printf("code gen from base class\n");
            return 0;
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
    virtual int codegen() {
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

class StmtAST:public AST {
    public:
    AST* value;
    StmtAST() {
    }
    ~StmtAST() {
    }
    virtual int codegen() {
        value->codegen();
        return 0;
    }
};
class FuncAST:public AST {
    public:
    FuncAST() {
    }
    ~FuncAST() {
    }
};

class StmtlistAST:public AST {
    public:
    vector<StmtAST*> stmts;
    StmtlistAST() {
    }
    ~StmtlistAST(){
    }
    virtual int codegen() {
        for (auto i:stmts) {
            i->codegen();
        }
        return 0;
    }
};

class ProgramAST:public AST {
    public:
};


#endif
