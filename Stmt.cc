#include "ast.h"
#include "Expr.h"
#include "Stmt.h"

//Stmt
Stmt::Stmt(): value(NULL) {
}

Stmt::Stmt(Expr* e):value(e) {
}

Stmt::~Stmt() {
}

void Stmt::codegen(BlockAST* block) {
    if (NULL != value) {
        value->codegen(block);
    }
}

std::string Stmt::Info() {
    return std::string("Statement");
}

void Stmt::Accept(Visitor* v) {
}

// ReturnStmt
ReturnStmt::ReturnStmt(Expr* e): Stmt(e) {
}

ReturnStmt::~ReturnStmt() {
}

void ReturnStmt::codegen(BlockAST* block) {
    Value *retval = value->codegen(block);
    if (NULL == retval) {
        printf("return stmt generate failed\n");
        return;
    }
    // stack object and global object need to load into register
    if (IsAllocaInst(retval) | IsGlobalVariable(retval)) {
        retval = Single::getBuilder()->CreateLoad(retval);
    }
    Single::getBuilder()->CreateRet(retval);
}

std::string ReturnStmt::Info() {
    return std::string("return statement");
}

//VarStmt
VarStmt::VarStmt(Expr* v): Stmt(v) {
}

VarStmt::~VarStmt() {
}

void VarStmt::codegen(BlockAST* block) {
    Value* p = value->codegen(block);
    p->getType()->dump();
}

// BlockAST
BlockAST::BlockAST():Parent(NULL), block(NULL) {
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

void BlockAST::addLocalVariable(std::string n, Value* v) {
    this->locals.insert(std::pair<std::string, Value*>(n, v));
}
Value* BlockAST::getLocalVariable(std::string n) {
    std::map<std::string, Value*>::iterator it;
    it = this->locals.find(n);
    if (it != this->locals.end()) {
        return it->second;
    }
    if (NULL == Parent) {
        return NULL;
    }
    return Parent->getLocalVariable(n);
}

void BlockAST::addStatement(Stmt* s) {
    this->stmts.push_back(s);
}
void BlockAST::setParent(BlockAST* p) {
    this->Parent = p;
}

void BlockAST::Accept(Visitor* v) {
    std::vector<Stmt*>::iterator it;
    for (it = stmts.begin(); it != stmts.end(); ++it) {
        v->VisitStmt(*it);
        (*it)->Accept(v);
    }
}

// FuncArgsAST
FuncArgsAST::FuncArgsAST() {
}

FuncArgsAST::~FuncArgsAST() {
}

void FuncArgsAST::addArg(VarExpr* v) {
    Names.push_back(v->getName());
    Args.push_back(v->getType());
    ParmVars.push_back(v);
}

std::vector<std::string> FuncArgsAST::getArgNames() {
    return Names;
}

size_t FuncArgsAST::getArgSize() {
    return Names.size();
}

std::vector<Type*> FuncArgsAST::getArgs() {
    return Args;
}
Type* FuncArgsAST::getArgType(size_t i) {
    if (i >= Args.size()) {
        return NULL;
    }
    return Args[i];
}

std::string FuncArgsAST::getArgName(size_t i) {
    if (i >= Names.size()) {
        return NULL;
    }
    return Names[i];
}

void FuncArgsAST::Accept(Visitor* v) {
    std::vector<VarExpr*>::iterator it;
    for (it = ParmVars.begin(); it != ParmVars.end(); ++it) {
        v->VisitVarExpr(*it);
    }
}

/**
 *Function
 *param[in] i      id of function name
 *param[in] rty    return type
 *param[in] args   args
 */
FuncProtoType::FuncProtoType(IdExpr* i, Type* rty, FuncArgsAST* args)
    :Id(i), ReturnTy(rty), Args(args) {
    }

FuncProtoType::~FuncProtoType(){
}

FunctionType* FuncProtoType::getFunctionType() {
    return FunctionType::get(this->ReturnTy, this->Args->getArgs(), false);
}

Function* FuncProtoType::codegen() {
    Function* F = Function::Create(this->getFunctionType(),
            Function::ExternalLinkage,
            this->getName(),
            Single::getModule());

    unsigned i = 0;
    Function::arg_iterator it;
    for (it = F->arg_begin(); i != this->getArgSize(); ++it, ++i) {
        it->setName(this->getArgName(i));
    }
    return F;
}

std::string FuncProtoType::getName() {
    return this->Id->getName();
}

size_t FuncProtoType::getArgSize() {
    return this->Args->getArgSize();
}

Type* FuncProtoType::getArgType(size_t i) {
    return Args->getArgType(i);
}

std::string FuncProtoType::getArgName(size_t i) {
    return Args->getArgName(i);
}

std::vector<Type*> FuncProtoType::getArgs() {
    return this->Args->getArgs();
}

void FuncProtoType::Accept(Visitor* v) {
    Args->Accept(v);
}

// FuncCallArgs
FuncCallArgs::FuncCallArgs() {
}

FuncCallArgs::~FuncCallArgs() {
}

void FuncCallArgs::pushArg(Expr* arg) {
    this->Args.push_back(arg);
}
std::vector<Value*> FuncCallArgs::getArgs(BlockAST* block) {
    std::vector<Value*> args;
    std::vector<Expr*>::iterator it;
    for (it = Args.begin(); it != Args.end(); ++it) {
        Value* p = (**it).codegen(block);
        if (IsAllocaInst(p)) {
            Value* r = Single::getBuilder()->CreateLoad(p);
            args.push_back(r);
        }else {
            args.push_back(p);
        }
    }
    return args;
}


// Func
Func::Func(FuncProtoType* f, BlockAST* b)
    :ProtoType(f), FuncBody(b){
    }

Func::~Func() {
}

//Function* Function::codegen() {
void Func::codegen(BlockAST* block) {
    // Set names for all arguments.
    Function* F = ProtoType->codegen();
    BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    this->FuncBody->block = BB;
    Single::getBuilder()->SetInsertPoint(BB);
    IRBuilder<> Builder(BB);

    unsigned i = 0;
    Function::arg_iterator it;
    for (it = F->arg_begin(); i != ProtoType->getArgSize(); ++it, ++i) {
        // My understand,  AI is in register, here store it to stack;
        //AllocaInst *stackvar = Builder.CreateAlloca(ProtoType->Args->args[i]);
        AllocaInst *StackVar = Builder.CreateAlloca(ProtoType->getArgType(i));
        Builder.CreateStore(&*it, StackVar);
        FuncBody->addLocalVariable(ProtoType->getArgName(i), StackVar);
    }
    if (NULL != this->FuncBody) {
        this->FuncBody->codegen();
    } else {
        printf("body is NULL\n");
    }
}

std::string Func::getName() {
    return ProtoType->getName();
}

std::string Func::Info() {
    char buf[20] = {0};
    snprintf(buf, 20, "Function [%s]", this->getName().c_str());
    return std::string(buf);
}

void Func::Accept(Visitor* v) {
    v->VisitFunc(this);
    ProtoType->Accept(v);
}

IfStmt::IfStmt(Expr* Cond, BlockAST* Then, BlockAST* Else)
    :Cond(Cond),Then(Then), Else(Else) {
    }
IfStmt::~IfStmt() {
}

void IfStmt::codegen(BlockAST* block) {
    Value* CondV = Cond->codegen(block);
    if (NULL == CondV) {
        return;
    }
    Value *Zero = ConstantInt::get(getGlobalContext(), APInt(32, 0));
    if (IsAllocaInst(CondV)) {
        CondV = Single::getBuilder()->CreateLoad(CondV);
    }
    CondV = Single::getBuilder()->CreateICmpNE(CondV, Zero);
    Function *TheFunction = Single::getBuilder()->GetInsertBlock()->getParent();
    BasicBlock *ThenBB = BasicBlock::Create(getGlobalContext(), "then", TheFunction);
    BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "else", TheFunction);
    Single::getBuilder()->CreateCondBr(CondV, ThenBB, ElseBB);
    Single::getBuilder()->SetInsertPoint(ThenBB);
    Then->setParent(block);
    Then->codegen();
    ThenBB = Single::getBuilder()->GetInsertBlock();

    Single::getBuilder()->CreateBr(ElseBB);
    Single::getBuilder()->SetInsertPoint(ElseBB);
    if (NULL != Else) {
        Else->setParent(block);
        Else->codegen();
    }
    ElseBB = Single::getBuilder()->GetInsertBlock();
}
