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

std::string IdExprAST::getName() {
    return this->name;
}

Value* IdExprAST::codegen(BlockAST* block) {
    std::map<std::string, Value*>::iterator it = block->locals.find(name);
    if (it != block->locals.end()) {
        Type* ty = it->second->getType();
        if (ty->isPointerTy()) {
            // this is pointer, allocated on stack, so used load
            return it->second;
            //return Single::getBuilder()->CreateLoad(it->second);
        }
        else {
            return it->second;
        }
    }
    it = Single::globalNamedValue.find(name);
    if (it != Single::globalNamedValue.end()) {
        return it->second;
    }
    printf("Can't find variable [%s]\n", name.c_str());
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
VarExprAST::VarExprAST(Type* ty, std::string n)
:type(ty), name(n) {
}

VarExprAST::~VarExprAST() {
}

Value* VarExprAST::codegen(BlockAST* block) {
    if (block->block != NULL) {
        // allocate on stack
        IRBuilder<> Builder(block->block);
        //AllocaInst *varalloc = Builder.CreateAlloca(Builder.getInt32Ty());
        AllocaInst *varalloc = Builder.CreateAlloca(this->type);
        varalloc->setName(this->name);
        block->locals.insert(std::pair<std::string, Value*>(varalloc->getName(), varalloc));
        return varalloc;
    }
    else {
        // global value
        GlobalVariable* p = new GlobalVariable(*Single::getModule(), Type::getInt32Ty(getGlobalContext()),
                false, Function::InternalLinkage, NULL);
        p->setName(this->name);
        // global value must be initialized
        Constant* initer = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0, true);
        p->setInitializer(initer);
        Single::globalNamedValue.insert(std::pair<std::string, Value*>(p->getName(), p));
        return p;
    }
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
            if (r->getType()->isPointerTy()) {
                r = Single::getBuilder()->CreateLoad(r);
            }
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
    if (NULL == retval) {
        printf("return stmt generate failed\n");
        return;
    }
    if (retval->getType()->isPointerTy()) {
        retval = Single::getBuilder()->CreateLoad(retval);
    }
    Single::getBuilder()->CreateRet(retval);
}

//VarStmtAST
VarStmtAST::VarStmtAST(VarExprAST* v): value(v) {
}

VarStmtAST::~VarStmtAST() {
}

void VarStmtAST::codegen(BlockAST* block) {
    value->codegen(block);
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

void FuncArgsAST::addarg(std::string name, Type* type) {
    names.push_back(name);
    args.push_back(type);
}

std::vector<std::string> FuncArgsAST::getNames() {
    return names;
}
std::vector<Type*> FuncArgsAST::getArgs() {
    return args;
}

/**
 *FuncAST
 *param[in] i      id of function name
 *param[in] rty    return type
 *param[in] args   args
 */
FuncProtoType::FuncProtoType(IdExprAST* i, Type* rty, FuncArgsAST* args)
    :Id(i), ReturnTy(rty), Args(args) {
}

FuncProtoType::~FuncProtoType(){
}

FunctionType* FuncProtoType::codegen() {
    return FunctionType::get(this->ReturnTy, this->Args->getArgs(), false);
}

std::string FuncProtoType::getName() {
    return this->Id->getName();
}

// FuncAST
FuncAST::FuncAST(FuncProtoType* f, BlockAST* b)
    :ProtoType(f), FuncBody(b){
}

FuncAST::~FuncAST() {
}

//Function* FuncAST::codegen() {
void FuncAST::codegen(BlockAST* block) {
    Function* F = Function::Create(this->ProtoType->codegen(),
            Function::ExternalLinkage,
            this->getName(),
            Single::getModule());
    // Set names for all arguments.
    unsigned Idx = 0;
    for (Function::arg_iterator AI = F->arg_begin();
            Idx != ProtoType->Args->names.size();
            ++AI, ++Idx) {
        AI->setName(ProtoType->Args->names[Idx]);
        FuncBody->locals.insert(std::pair<std::string, Value*>(ProtoType->Args->names[Idx], AI));
    }
    BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    this->FuncBody->block = BB;
    Single::getBuilder()->SetInsertPoint(BB);
    if (NULL != this->FuncBody) {
        this->FuncBody->codegen();
    } else {
        printf("body is NULL\n");
    }
}

std::string FuncAST::getName() {
    return ProtoType->getName();
}
