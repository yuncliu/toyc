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

bool DumpVisitor::Visit(Stmt* s) {
    VISIT_FUNC visit_func = this->getFunction(s->getSelfName());
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

bool DumpVisitor::VisitCompoundStmt(Stmt* stmt) {
    CompoundStmt* p = (CompoundStmt*)stmt;
    printf("%sConmpoundStmt:%s\n", LIGHT_GREEN, COLOR_RESET);
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

bool DumpVisitor::VisitFunc(Stmt* stmt) {
    Func* f = (Func*)stmt;
    printf("%sFunction%s: name [%s] return type [%s]\n",LIGHT_GREEN, COLOR_RESET,
            f->ProtoType->Id->Id.c_str(),
            f->ProtoType->ReturnTy->Type.c_str());
    this->prefix.push_back("|-");
    this->Visit(f->ProtoType);
    is_last = true;
    this->Visit(f->FuncBody);
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitFuncProtoType(Stmt* stmt) {
    printf("%sFunctionProtoType%s\n", LIGHT_GREEN, COLOR_RESET);
    FuncProtoType* f = (FuncProtoType*)stmt;
    this->prefix.push_back("|-");
    this->Visit(f->Id);
    this->Visit(f->ReturnTy);
    is_last = true;;
    this->Visit(f->Param);
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitIdExpr(Stmt* stmt) {
    IdExpr* p = static_cast<IdExpr*>(stmt);
    printf("ID:[%s]\n", p->Id.c_str());
    return true;
}

bool DumpVisitor::VisitTypeExpr(Stmt* stmt) {
    TypeExpr* p = static_cast<TypeExpr*>(stmt);
    printf("TypeExpr: [%s]\n", p->Type.c_str());
    return true;
}

bool DumpVisitor::VisitFuncParameter(Stmt* stmt) {
    FuncParameter* p = (FuncParameter*)stmt;
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

bool DumpVisitor::VisitVarExpr(Stmt* stmt) {
    VarExpr* p = (VarExpr*)stmt;
    printf("VarExpr: Type[%s] Id[%s]\n", p->Type->Type.c_str(), p->Id->Id.c_str());
    //this->Visit(p->Type);
    //this->Visit(p->Id);
    return true;
}

bool DumpVisitor::VisitBinaryExpr(Stmt* stmt) {
    BinaryExpr* p = (BinaryExpr*)stmt;
    printf("BinaryExpr: [%c]\n", p->op);
    this->prefix.push_back("|-");
    this->Visit(p->left);
    is_last = true;
    this->Visit(p->right);
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitReturnStmt(Stmt* stmt) {
    ReturnStmt* p = static_cast<ReturnStmt*>(stmt);
    printf("ReturnStmt:\n");
    this->prefix.push_back("|-");
    is_last = true;
    this->Visit(p->Ret);
    this->prefix.pop_back();
    return true;
}

bool DumpVisitor::VisitIntExpr(Stmt* stmt) {
    IntExpr* p = static_cast<IntExpr*>(stmt);
    printf("IntExpr Value = [%d]\n", p->value);
    return true;
}
