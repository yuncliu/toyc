#include "ast.h"
#include "Expr.h"
#include "Stmt.h"
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
