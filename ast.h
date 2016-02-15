/**
 * int a = 0;
 * got a VarAST x
 * x.type   = IntAST
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
class TypeAST;
class AST {
    public:
        AST() {
        }
        virtual ~AST() {
        }
};

class IdAST:public AST{
    public:
    string name;
    IdAST(string s) :name(s){
    }
    virtual ~IdAST() {
    }
};

class VarAST:public AST{
    public:
        TypeAST*    type;
        IdAST*      id;
        AST*        value;
        VarAST(TypeAST* t, IdAST* i)
            :type(t), id(i), value(NULL) {
        }
        VarAST(TypeAST* t, IdAST* i, AST* v)
        :type(t), id(i), value(v) {
        }
        ~VarAST() {
        }
};

class TypeAST:public AST {
    public:
        string type;
        TypeAST(string s): type(s) {
        }
        ~TypeAST() {
        }
};

class IntAST:public AST {
    int value;
    public:
    IntAST(int i) :value(i) {
    }
    ~IntAST(){
    }
};

class ExprAST:public AST {
    char op;
    public:
    ExprAST() {
    }
    ~ExprAST() {
    }
};

class StmtAST:public AST {
    int i;
    public:
    StmtAST() {
    }
    ~StmtAST() {
    }
};

class StmtlistAST:public AST {
    vector<AST*> stmts;
    public:
    StmtlistAST() {
    }
    ~StmtlistAST(){
    }
};

class ProgramAST:public AST {
    public:
};


#endif
