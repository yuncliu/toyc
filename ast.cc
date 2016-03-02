#include "ast.h"

bool IsAllocaInst(Value* v) {
    return v->getValueID() == Value::InstructionVal + Instruction::Alloca;
}

bool IsGlobalVariable(Value* v) {
    return v->getValueID() == Value::GlobalVariableVal;
}

// Expr
Expr::Expr() {
}

Expr::~Expr() {
}

Value* Expr::codegen(BlockAST* block) {
    return NULL;
}

// IdExpr
IdExpr::IdExpr(std::string s) :name(s){
}

IdExpr::~IdExpr() {
}

std::string IdExpr::getName() {
    return this->name;
}

Value* IdExpr::codegen(BlockAST* block) {
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

// IntExpr
IntExpr::IntExpr(int i): value(i) {
}

IntExpr::~IntExpr() {
}

Value* IntExpr::codegen(BlockAST* block) {
    return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value, true);
}

DoubleExpr::DoubleExpr(double d)
    :value(d) {
}

DoubleExpr::~DoubleExpr() {
}

Value* DoubleExpr::codegen(BlockAST* block) {
    printf("double code gen [%f]\n", value);
    return ConstantFP::get(getGlobalContext(), APFloat(value));
}

//VarExpr
VarExpr::VarExpr(Type* ty, IdExpr* id)
:type(ty), Id(id) {
}

VarExpr::~VarExpr() {
}

Value* VarExpr::codegen(BlockAST* block) {
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

std::string VarExpr::getName() {
    return Id->getName();
}

Type* VarExpr::getType() {
    return type;
}

std::string VarExpr::Info() {
    char buf[20] = {0};
    snprintf(buf, 20, "Var %s", Id->getName().c_str());
    return std::string(buf);
}

FuncCallExpr::FuncCallExpr(IdExpr* id, FuncCallArgs* args)
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

// BinaryExpr
BinaryExpr::BinaryExpr(char op, Expr* l, Expr* r)
    :op(op), left(l), right(r) {
    }

BinaryExpr::~BinaryExpr() {
}

Value* BinaryExpr::codegen(BlockAST* block) {
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
            if (l->getType()->isIntegerTy() && r->getType()->isIntegerTy()) {
                return Single::getBuilder()->CreateAdd(l, r);
            }
            else {
                return Single::getBuilder()->CreateFAdd(l, r);
            }
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
            if (l->getType()->isIntegerTy() && r->getType()->isIntegerTy()) {
                return Single::getBuilder()->CreateSub(l, r);
            } else {
                return Single::getBuilder()->CreateFSub(l, r);
            }
            break;
        case '*':
            l = left->codegen(block);
            r = right->codegen(block);
            if (IsAllocaInst(l)) {
                l = Single::getBuilder()->CreateLoad(l);
            }
            if (IsAllocaInst(r)) {
                r = Single::getBuilder()->CreateLoad(r);
            }
            if (l->getType()->isIntegerTy() && r->getType()->isIntegerTy()) {
                return Single::getBuilder()->CreateMul(l, r);
            } else {
                return Single::getBuilder()->CreateFMul(l, r);
            }
            break;
        case '/':
            l = left->codegen(block);
            r = right->codegen(block);
            if (IsAllocaInst(l)) {
                l = Single::getBuilder()->CreateLoad(l);
            }
            if (IsAllocaInst(r)) {
                r = Single::getBuilder()->CreateLoad(r);
            }
            if (l->getType()->isIntegerTy() && r->getType()->isIntegerTy()) {
                return Single::getBuilder()->CreateSDiv(l, r);
            } else {
                return Single::getBuilder()->CreateFDiv(l, r);
            }
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
                return NULL;
                //return Single::getBuilder()->CreateStore(r, l);
            }

            if (((AllocaInst*)l)->getAllocatedType()->getTypeID() == r->getType()->getTypeID()) {
                return Single::getBuilder()->CreateStore(r, l);
            }
            if (((AllocaInst*)l)->getAllocatedType()->isIntegerTy()
                    && (r->getType()->isDoubleTy() || r->getType()->isFloatTy())) {
               r = Single::getBuilder()->CreateFPToSI(r, ((AllocaInst*)l)->getAllocatedType());
            }
            if (((AllocaInst*)l)->getAllocatedType()->isDoubleTy()
                && r->getType()->isIntegerTy()) {
               r = Single::getBuilder()->CreateSIToFP(r, ((AllocaInst*)l)->getAllocatedType());
            }
            return Single::getBuilder()->CreateStore(r, l);
            break;
        default:
            break;
    }
    return NULL;
}

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
