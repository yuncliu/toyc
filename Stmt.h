#ifndef _STMT_H_
#define _STMT_H_

class Stmt {
    protected:
        Expr* value;
    public:
        Stmt();
        Stmt(Expr* e);
        ~Stmt();
        virtual void codegen(BlockAST* block);
        virtual std::string Info();
        virtual void Accept(Visitor* v);
};

class ReturnStmt: public Stmt {
    public:
        ReturnStmt(Expr* e);
        ~ReturnStmt();
        virtual void codegen(BlockAST* block);
        std::string Info();
};

class VarStmt: public Stmt {
    public:
        VarStmt(Expr* v);
        ~VarStmt();
        virtual void codegen(BlockAST* block);
};

class BlockAST {
        std::vector<Stmt*> stmts;
        std::map<std::string, Value*> locals;
        BlockAST* Parent;
    public:
        BasicBlock* block;
        BlockAST();
        ~BlockAST();
        virtual void codegen();
        void addLocalVariable(std::string n, Value* v);
        Value* getLocalVariable(std::string n);
        void addStatement(Stmt* s);
        void setParent(BlockAST* p);
        void Accept(Visitor* v);
};

class FuncArgsAST {
        std::vector<std::string> Names;
        std::vector<Type*>  Args;
        std::vector<VarExpr*>  ParmVars;
    public:
        FuncArgsAST();
        ~FuncArgsAST();
        void addArg(VarExpr* v);
        std::vector<std::string> getArgNames();
        size_t getArgSize();
        std::vector<Type*> getArgs();
        Type* getArgType(size_t i);
        std::string getArgName(size_t i);
        void Accept(Visitor* v);
};

class FuncProtoType {
        IdExpr* Id;
        Type* ReturnTy;
        FuncArgsAST* Args;
    public:
        FuncProtoType(IdExpr* i, Type* rty, FuncArgsAST* args);
        ~FuncProtoType();
        FunctionType* getFunctionType();
        Function* codegen();
        std::string getName();
        size_t getArgSize();
        Type* getArgType(size_t i);
        std::string getArgName(size_t i);
        std::vector<Type*> getArgs();
        void Accept(Visitor* v);
};

class FuncCallArgs {
    std::vector<Expr*> Args;
    public:
    FuncCallArgs();
    ~FuncCallArgs();
    void pushArg(Expr* arg);
    std::vector<Value*> getArgs(BlockAST* block);
};

class Func: Stmt {
        FuncProtoType* ProtoType;
        BlockAST* FuncBody;
    public:
        Func(FuncProtoType* f, BlockAST* b);
        ~Func();
        virtual void codegen(BlockAST* block);
        std::string getName();
        virtual std::string Info();
        virtual void Accept(Visitor* v);
};

class IfStmt: Stmt {
    Expr* Cond;
    BlockAST* Then;
    BlockAST* Else;
    public:
    IfStmt(Expr* Cond, BlockAST* Then, BlockAST* Else);
    ~IfStmt();
    virtual void codegen(BlockAST* block);
};
#endif // _STMT_H_
