#ifndef _DUMP_VISITOR_H_
#define _DUMP_VISITOR_H_
#include <map>
#include <vector>
#include "Visitor.h"
#define LIGHT_GREEN "\x1B[0;1;32m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_RESET "\033[0m"
class DumpVisitor;
class Stmt;
typedef bool (DumpVisitor::*VISIT_FUNC)(Stmt* s);

class DumpVisitor:public Visitor {
    protected:
        std::map<std::string, VISIT_FUNC> functions;
        std::vector<std::string> prefix;
        bool is_last;
    public:
        DumpVisitor();
        virtual ~DumpVisitor();
        VISIT_FUNC getFunction(std::string name);
        virtual bool Visit(Stmt*);
        void print_prefix();
        bool VisitCompoundStmt(Stmt* stmt);
        bool VisitFunc(Stmt* stmt);
        bool VisitFuncProtoType(Stmt* stmt);
        bool VisitIdExpr(Stmt* stmt);
        bool VisitTypeExpr(Stmt* stmt);
        bool VisitFuncParameter(Stmt* stmt);
        bool VisitVarExpr(Stmt* stmt);
        bool VisitBinaryExpr(Stmt* stmt);
        bool VisitReturnStmt(Stmt* stmt);
        bool VisitIntExpr(Stmt* stmt);
};

#endif // _VISITOR_H_
