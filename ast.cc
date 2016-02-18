#include "ast.h"

// ExprAST
ExprAST::ExprAST() {
}

ExprAST::~ExprAST() {
}

Value* ExprAST::codegen(BlockAST* block) {
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
        Type* ty = it->second->getType();
        if (ty->isPointerTy()) {
            // this is pointer, allocated on stack, so used load
            return Single::getBuilder()->CreateLoad(it->second);
        }
        else {
            return it->second;
        }
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
BinaryExprAST::BinaryExprAST(char op, ExprAST* l, ExprAST* r)
:op(op), left(l), right(r) {
}

BinaryExprAST::~BinaryExprAST() {
}

Value* BinaryExprAST::codegen(BlockAST* block) {
    Value* l = NULL;
    Value* r = NULL;
    switch(this->op) {
        case '+':
            l = left->codegen(block);
            r = right->codegen(block);
            return Single::getBuilder()->CreateAdd(l, r);
            break;
        case '=':
            l = left->codegen(block);
            r = right->codegen(block);
            return Single::getBuilder()->CreateStore(r, l);
            break;
        default:
            break;
    }
    return NULL;
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
        value->codegen(block);
    }
}

// ReturnStmtAST
ReturnStmtAST::ReturnStmtAST(ExprAST* e): expr(e) {
}

ReturnStmtAST::~ReturnStmtAST() {
}

void ReturnStmtAST::codegen(BlockAST* block) {
    Value *retval = expr->codegen(block);
    Single::getBuilder()->CreateRet(retval);
}

//VarStmtAST
VarStmtAST::VarStmtAST(ExprAST* e): var(e) {
}

VarStmtAST::~VarStmtAST() {
}

void VarStmtAST::codegen(BlockAST* block) {
    if (block->block != NULL) {
        IRBuilder<> Builder(block->block);
        AllocaInst *varalloc = Builder.CreateAlloca(Builder.getInt32Ty());
        varalloc->setName(((VarExprAST*)var)->name);
        block->locals.insert(std::pair<std::string, Value*>(varalloc->getName(), varalloc));
    }
}

// BlockAST
BlockAST::BlockAST():block(NULL) {
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

//Function* FuncAST::codegen() {
void FuncAST::codegen(BlockAST* block) {
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
    this->body->block = BB;
    Single::getBuilder()->SetInsertPoint(BB);
    if (NULL != body) {
        body->codegen();
    } else {
        printf("body is NULL\n");
    }
}
void FuncAST::addbody(BlockAST* b) {
    this->body = b;
}
