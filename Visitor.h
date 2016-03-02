#ifndef _VISITOR_H_
#define _VISITOR_H_
#include "ast.h"

class Expr;
class Stmt;
class Func;

class Visitor {
    public:
        Visitor();
        virtual ~Visitor();
        void VisitExpr(Expr* expr);
        void VisitStmt(Stmt* stmt);
        void VisitFunc(Func* func);
};

#endif // _VISITOR_H_
