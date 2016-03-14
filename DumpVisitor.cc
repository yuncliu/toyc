#include "DumpVisitor.h"
#include "Expr.h"
#include "Stmt.h"

DumpVisitor::DumpVisitor():is_last(false) {
    functions.insert(std::pair<std::string, VISIT_FUNC>("CompoundStmt", &DumpVisitor::VisitCompoundStmt));
    functions.insert(std::pair<std::string, VISIT_FUNC>("Func", &DumpVisitor::VisitFunc));
    functions.insert(std::pair<std::string, VISIT_FUNC>("FuncProtoType", &DumpVisitor::VisitFuncProtoType));
    functions.insert(std::pair<std::string, VISIT_FUNC>("IdExpr", &DumpVisitor::VisitIdExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("TypeExpr", &DumpVisitor::VisitTypeExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("FuncParameter", &DumpVisitor::VisitFuncParameter));
    functions.insert(std::pair<std::string, VISIT_FUNC>("VarExpr", &DumpVisitor::VisitVarExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("BinaryExpr", &DumpVisitor::VisitBinaryExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("ReturnStmt", &DumpVisitor::VisitReturnStmt));
    functions.insert(std::pair<std::string, VISIT_FUNC>("IntExpr", &DumpVisitor::VisitIntExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("FuncCallExpr", &DumpVisitor::VisitFuncCallExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("IfStmt", &DumpVisitor::VisitIfStmt));
}

DumpVisitor::~DumpVisitor() {
}

VISIT_FUNC DumpVisitor::getFunction(std::string name) {
    std::map<std::string, VISIT_FUNC>::iterator it = functions.find(name);
    if (it == functions.end() ){
        return NULL;
    }
    return it->second;
}

bool DumpVisitor::Visit(std::shared_ptr<Stmt> s) {
    std::string a = s->getSelfName();
    VISIT_FUNC visit_func = this->getFunction(a);
    if (NULL == visit_func) {
        printf("No visit function for [%s]\n", s->getSelfName().c_str());
        return false;
    }
    print_prefix();
    is_last = false;
    return (this->*visit_func)(s);
}
/**
 *print out prefix,
 *ConmpoundStmt:
 *`-Function: name [main] return type [int]   prefix="`-"
 *  |-FunctionProtoType                       prefix="  |-"
 *  | |-ID:[main]                             prefix="  | |-"
 *  | |-TypeExpr: [int]
 *  | `-No Parameters
 *  `-ConmpoundStmt:
 *    `-ReturnStmt:
 *      `-IntExpr Value = [0]
 */
void DumpVisitor::print_prefix() {
    printf(COLOR_BLUE); //Blue
    int size = prefix.size();
    for (int i = 0; i < size; ++i) {
        std::string t = prefix[i];
        if (t == "|-" && i != size-1) {
            t = "| ";
        }
        if (is_last && i == size-1) {
            t = "`-";
            prefix[i] = "  ";
        }
        printf("%s", t.c_str());
    }
    printf(COLOR_RESET);
}

bool DumpVisitor::VisitCompoundStmt(std::shared_ptr<Stmt> stmt) {
    //CompoundStmt* p = (CompoundStmt*)stmt;
    std::shared_ptr<CompoundStmt> p = std::static_pointer_cast<CompoundStmt>(stmt);
    printf("%sCompoundStmt:%s\n", LIGHT_GREEN, COLOR_RESET);
    this->prefix.push_back("|-");
    int size = p->stmts.size();
    for (int i = 0; i < size; ++i) {
        if (i == size - 1) {
            is_last = true;
        }
        this->Visit(p->stmts[i]);
    }
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitFunc(std::shared_ptr<Stmt> stmt) {
    //Func* f = (Func*)stmt;
    std::shared_ptr<Func> p = std::static_pointer_cast<Func>(stmt);
    printf("%sFunction%s: name [%s] return type [%s]\n",LIGHT_GREEN, COLOR_RESET,
            p->ProtoType->Id->Id.c_str(),
            p->ProtoType->ReturnTy->Type.c_str());
    this->prefix.push_back("|-");
    this->Visit(p->ProtoType);
    is_last = true;
    this->Visit(p->FuncBody);
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitFuncProtoType(std::shared_ptr<Stmt> stmt) {
    printf("%sFunctionProtoType%s\n", LIGHT_GREEN, COLOR_RESET);
    //FuncProtoType* f = (FuncProtoType*)stmt;
    std::shared_ptr<FuncProtoType> p = std::static_pointer_cast<FuncProtoType>(stmt);
    this->prefix.push_back("|-");
    this->Visit(p->Id);
    this->Visit(p->ReturnTy);
    is_last = true;;
    this->Visit(p->Param);
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitIdExpr(std::shared_ptr<Stmt> stmt) {
    //IdExpr* p = static_cast<IdExpr*>(stmt);
    std::shared_ptr<IdExpr> p = std::static_pointer_cast<IdExpr>(stmt);
    printf("ID:[%s]\n", p->Id.c_str());
    return true;
}

bool DumpVisitor::VisitTypeExpr(std::shared_ptr<Stmt> stmt) {
    //TypeExpr* p = static_cast<TypeExpr*>(stmt);
    std::shared_ptr<TypeExpr> p = std::static_pointer_cast<TypeExpr>(stmt);
    printf("TypeExpr: [%s]\n", p->Type.c_str());
    return true;
}

bool DumpVisitor::VisitFuncParameter(std::shared_ptr<Stmt> stmt) {
    //FuncParameter* p = (FuncParameter*)stmt;
    std::shared_ptr<FuncParameter> p = std::static_pointer_cast<FuncParameter>(stmt);
    if (p->Params.size() == 0) {
        printf("No Parameters\n");
        return true;
    }
    printf("FuncParameter\n");
    this->prefix.push_back("|-");
    int size = p->Params.size();
    for (int i = 0; i < size; ++i) {
        if (i == size - 1) {
            is_last = true;
        }
        this->Visit(p->Params[i]);
    }
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitVarExpr(std::shared_ptr<Stmt> stmt) {
    //VarExpr* p = (VarExpr*)stmt;
    std::shared_ptr<VarExpr> p = std::static_pointer_cast<VarExpr>(stmt);
    printf("VarExpr: Type[%s] Id[%s]\n", p->Type->Type.c_str(), p->Id->Id.c_str());
    //this->Visit(p->Type);
    //this->Visit(p->Id);
    return true;
}

bool DumpVisitor::VisitBinaryExpr(std::shared_ptr<Stmt> stmt) {
    //BinaryExpr* p = (BinaryExpr*)stmt;
    std::shared_ptr<BinaryExpr> p = std::static_pointer_cast<BinaryExpr>(stmt);
    printf("BinaryExpr: [%c]\n", p->op);
    this->prefix.push_back("|-");
    this->Visit(p->left);
    is_last = true;
    this->Visit(p->right);
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitReturnStmt(std::shared_ptr<Stmt> stmt) {
    //ReturnStmt* p = static_cast<ReturnStmt*>(stmt);
    std::shared_ptr<ReturnStmt> p = std::static_pointer_cast<ReturnStmt>(stmt);
    printf("ReturnStmt:\n");
    this->prefix.push_back("|-");
    is_last = true;
    this->Visit(p->Ret);
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitIntExpr(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<IntExpr> p = std::static_pointer_cast<IntExpr>(stmt);
    printf("IntExpr Value = [%d]\n", p->value);
    return true;
}

bool DumpVisitor::VisitFuncCallExpr(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<FuncCallExpr> p = std::static_pointer_cast<FuncCallExpr>(stmt);
    printf("Function Call: [%s]\n", p->Id->Id.c_str());
    this->prefix.push_back("|-");
    for (size_t i = 0; i < p->Args->Parameters.size(); ++i) {
        if (i == p->Args->Parameters.size() - 1) {
            is_last = true;
        }
        this->Visit(p->Args->Parameters[i]);
    }
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitIfStmt(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<IfStmt> p = std::static_pointer_cast<IfStmt>(stmt);
    printf("If Stmt\n");
    this->prefix.push_back("|-");
    this->Visit(p->Cond);
    if (!p->Else) {
        is_last = true;
    }
    this->Visit(p->Then);
    if (p->Else) {
        is_last = true;
        this->Visit(p->Else);
    }
    this->prefix.pop_back();
    return true;
}
