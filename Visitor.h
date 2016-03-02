#ifndef _VISITOR_H_
#define _VISITOR_H_
#include "ast.h"

class Visitor {
    public:
        Visitor();
        virtual ~Visitor();
        void VisitExpr(ExprAST* expr);
};

#endif // _VISITOR_H_
