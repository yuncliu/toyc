#include "ast.h"

// ExprAST
ExprAST::ExprAST() {
}

ExprAST::~ExprAST() {
}

Value* ExprAST::codegen(BlockAST* block) {
    return NULL;
}

Value* ExprAST::codegen() {
    return NULL;
}

// IdExprAST
IdExprAST::IdExprAST(std::string s) :name(s){
}

IdExprAST::~IdExprAST() {
}

Value* IdExprAST::codegen(BlockAST* block) {
    std::map<std::string, Value*>::iterator it = block->locals.find(name);
    if (it != block->locals.end()) {
        return it->second;
    }
    return NULL;
}

// IntExprAST
IntExprAST::IntExprAST(int i): value(i) {
}

IntExprAST::~IntExprAST() {
}

Value* IntExprAST::codegen(BlockAST* block) {
    return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value, true);
}

//VarExprAST
VarExprAST::VarExprAST(std::string t, std::string n)
:type(t), name(n) {
}

VarExprAST::~VarExprAST() {
}

Value* VarExprAST::codegen(BlockAST* block) {
    return NULL;
}

// BinaryExprAST
BinaryExprAST::BinaryExprAST(ExprAST* l, ExprAST* r)
:left(l), right(r) {
}

BinaryExprAST::~BinaryExprAST() {
}

Value* BinaryExprAST::codegen(BlockAST* block) {
    Value* l = left->codegen();
    Value* r = right->codegen();
    return Single::getBuilder()->CreateAdd(l, r);
}

//StmtAST
StmtAST::StmtAST(): value(NULL) {
}

StmtAST::StmtAST(ExprAST* e):value(e) {
}

StmtAST::~StmtAST() {
}

void StmtAST::codegen(BlockAST* block) {
    if (NULL != value) {
        value->codegen();
    }
    Value *retval = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 10, true);
    Single::getBuilder()->CreateRet(retval);
}

// ReturnStmtAST
ReturnStmtAST::ReturnStmtAST(ExprAST* e): expr(e) {
}

ReturnStmtAST::~ReturnStmtAST() {
}

void ReturnStmtAST::codegen(BlockAST* block) {
    printf("codegen of Return Stmt\n");
    Value *retval = expr->codegen(block);
    Single::getBuilder()->CreateRet(retval);
}

// BlockAST
BlockAST::BlockAST() {
}
BlockAST::~BlockAST() {
}

void BlockAST::codegen() {
    if (stmts.size() == 0 ) {
        printf("no statements\n");
    } else {
        printf("%ld statements\n", stmts.size());
    }
    for (auto i:stmts) {
        i->codegen(this);
    }
}

// FuncArgsAST
FuncArgsAST::FuncArgsAST() {
}
FuncArgsAST::~FuncArgsAST() {
}

void FuncArgsAST::addarg(std::string name, std::string type) {
    names.push_back(name);
    if (type == "int") {
        args.push_back(Type::getInt32Ty(getGlobalContext()));
    }
}
std::vector<std::string> FuncArgsAST::getNames() {
    return names;
}
std::vector<Type*> FuncArgsAST::getArgs() {
    return args;
}

// FuncTypeAST
FuncTypeAST::FuncTypeAST() {
}

FuncTypeAST::~FuncTypeAST(){
}

FunctionType* FuncTypeAST::codegen() {
    FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()),
            arg_list->getArgs(), false);
    return FT;
}

// FuncAST
FuncAST::FuncAST(std::string n)
:name(n) {
    functype = new FuncTypeAST();
}

FuncAST::~FuncAST() {
}

Function* FuncAST::codegen() {
    Function* F = Function::Create(functype->codegen(),
            Function::ExternalLinkage,
            name,
            Single::getModule());
    // Set names for all arguments.
    unsigned Idx = 0;
    for (Function::arg_iterator AI = F->arg_begin();
            Idx != functype->arg_list->names.size();
            ++AI, ++Idx) {
        AI->setName(functype->arg_list->names[Idx]);
        body->locals.insert(std::pair<std::string, Value*>(functype->arg_list->names[Idx], AI));
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
