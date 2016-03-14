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
typedef bool (DumpVisitor::*VISIT_FUNC)(std::shared_ptr<Stmt> s);

class DumpVisitor:public Visitor {
    protected:
        std::map<std::string, VISIT_FUNC> functions;
        std::vector<std::string> prefix;
        bool is_last;
    public:
        DumpVisitor();
        virtual ~DumpVisitor();
        VISIT_FUNC getFunction(std::string name);
        virtual bool Visit(std::shared_ptr<Stmt> s);
        void print_prefix();
        bool VisitCompoundStmt(std::shared_ptr<Stmt> stmt);
        bool VisitFunc(std::shared_ptr<Stmt> stmt);
        bool VisitFuncProtoType(std::shared_ptr<Stmt> stmt);
        bool VisitIdExpr(std::shared_ptr<Stmt> stmt);
        bool VisitTypeExpr(std::shared_ptr<Stmt> stmt);
        bool VisitFuncParameter(std::shared_ptr<Stmt> stmt);
        bool VisitVarExpr(std::shared_ptr<Stmt> stmt);
        bool VisitBinaryExpr(std::shared_ptr<Stmt> stmt);
        bool VisitReturnStmt(std::shared_ptr<Stmt> stmt);
        bool VisitIntExpr(std::shared_ptr<Stmt> stmt);
        bool VisitFuncCallExpr(std::shared_ptr<Stmt> stmt);
        bool VisitIfStmt(std::shared_ptr<Stmt> stmt);
};

#endif // _VISITOR_H_
