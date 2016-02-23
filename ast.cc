#include "ast.h"

bool IsAllocaInst(Value* v) {
    return v->getValueID() == Value::InstructionVal + Instruction::Alloca;
}

bool IsGlobalVariable(Value* v) {
    return v->getValueID() == Value::GlobalVariableVal;
}

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
    //std::map<std::string, Value*>::iterator it = block->locals.find(name);
    Value* v = block->getLocalVariable(name);
    if (NULL != v) {
        return v;
    }
    std::map<std::string, Value*>::iterator it;
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
VarExprAST::VarExprAST(Type* ty, IdExprAST* id)
:type(ty), Id(id) {
}

VarExprAST::~VarExprAST() {
}

Value* VarExprAST::codegen(BlockAST* block) {
    if (block->block != NULL) {
        // allocate on stack
        IRBuilder<> Builder(block->block);
        AllocaInst* Var = Builder.CreateAlloca(this->type);
        Var->setName(this->getName());
        block->addLocalVariable(Var->getName(), Var);
        return Var;
    }
    else {
        // global value
        GlobalVariable* p = new GlobalVariable(*Single::getModule(), Type::getInt32Ty(getGlobalContext()),
                false, Function::InternalLinkage, NULL);
        p->setName(this->getName());
        // global value must be initialized
        Constant* initer = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0, true);
        p->setInitializer(initer);
        Single::globalNamedValue.insert(std::pair<std::string, Value*>(p->getName(), p));
        return p;
    }
    return NULL;
}

std::string VarExprAST::getName() {
    return Id->getName();
}

Type* VarExprAST::getType() {
    return type;
}

FuncCallExpr::FuncCallExpr(IdExprAST* id, FuncCallArgs* args)
:Id(id), Args(args) {
}

FuncCallExpr::~FuncCallExpr() {
}

Value* FuncCallExpr::codegen(BlockAST* block) {
    Function* func = Single::getModule()->getFunction(Id->getName().c_str());
    if (NULL == func) {
        printf("not such function: [%s]", Id->getName().c_str());
        return NULL;
    }
    std::vector<Value*> args = this->Args->getArgs(block);
    CallInst *call = CallInst::Create(func, makeArrayRef(args), "", block->block);
    return call;
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
            if (IsAllocaInst(l)) {
                l = Single::getBuilder()->CreateLoad(l);
            }
            if (IsAllocaInst(r)) {
                r = Single::getBuilder()->CreateLoad(r);
            }
            return Single::getBuilder()->CreateAdd(l, r);
            break;
        case '-':
            l = left->codegen(block);
            r = right->codegen(block);
            if (IsAllocaInst(l)) {
                l = Single::getBuilder()->CreateLoad(l);
            }
            if (IsAllocaInst(r)) {
                r = Single::getBuilder()->CreateLoad(r);
            }
            return Single::getBuilder()->CreateSub(l, r);
            break;
        case '=':
            l = left->codegen(block);
            r = right->codegen(block);
            if (IsAllocaInst(r)) {
                r = Single::getBuilder()->CreateLoad(r);
            }
            if (IsGlobalVariable(l)) {
                printf("get a global valriable\n");
                ((GlobalVariable*)l)->setInitializer((Constant*)r);
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
    // stack object and global object need to load into register
    if (IsAllocaInst(retval) | IsGlobalVariable(retval)) {
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

void BlockAST::addStatement(StmtAST* s) {
    this->stmts.push_back(s);
}
void BlockAST::setParent(BlockAST* p) {
    this->Parent = p;
}

// FuncArgsAST
FuncArgsAST::FuncArgsAST() {
}

FuncArgsAST::~FuncArgsAST() {
}

void FuncArgsAST::addArg(VarExprAST* v) {
    Names.push_back(v->getName());
    Args.push_back(v->getType());
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

// FuncCallArgs
FuncCallArgs::FuncCallArgs() {
}

FuncCallArgs::~FuncCallArgs() {
}

void FuncCallArgs::pushArg(ExprAST* arg) {
    this->Args.push_back(arg);
}
std::vector<Value*> FuncCallArgs::getArgs(BlockAST* block) {
    std::vector<Value*> args;
    std::vector<ExprAST*>::iterator it;
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


// FuncAST
FuncAST::FuncAST(FuncProtoType* f, BlockAST* b)
    :ProtoType(f), FuncBody(b){
    }

FuncAST::~FuncAST() {
}

//Function* FuncAST::codegen() {
void FuncAST::codegen(BlockAST* block) {
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
        Builder.CreateStore(it, StackVar);
        FuncBody->addLocalVariable(ProtoType->getArgName(i), StackVar);
    }
    if (NULL != this->FuncBody) {
        this->FuncBody->codegen();
    } else {
        printf("body is NULL\n");
    }
}

std::string FuncAST::getName() {
    return ProtoType->getName();
}

IfStmtAST::IfStmtAST(ExprAST* Cond, BlockAST* block)
    :Cond(Cond),Body(block) {
}
IfStmtAST::~IfStmtAST() {
}

void IfStmtAST::codegen(BlockAST* block) {
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
    Body->setParent(block);
    Body->codegen();
    ThenBB = Single::getBuilder()->GetInsertBlock();

    Single::getBuilder()->CreateBr(ElseBB);
    Single::getBuilder()->SetInsertPoint(ElseBB);
    ElseBB = Single::getBuilder()->GetInsertBlock();
}
