#ifndef _VISITOR_H_
#define _VISITOR_H_
#include <map>
class Expr;
class Stmt;
class Func;
class FuncProtoType;
class CompoundStmt;
class VarExpr;
class Visitor;
typedef bool (Visitor::*TRAVEL_FUNC)(Stmt* s);

class Visitor {
    protected:
    std::map<std::string, TRAVEL_FUNC> functions;
    public:
        int indent;
        Visitor();
        virtual ~Visitor();
        TRAVEL_FUNC getFunction(std::string name);
        bool Travel(Stmt*);
        bool TravelCompoundStmt(Stmt* stmt);
        bool TravelFunc(Stmt* stmt);
        bool TravelFuncProtoType(Stmt* stmt);
        bool TravelIdExpr(Stmt* stmt);
        bool TravelTypeExpr(Stmt* stmt);
        bool TravelFuncParameter(Stmt* stmt);
        bool TravelVarExpr(Stmt* stmt);
        bool TravelBinaryExpr(Stmt* stmt);
        bool TravelReturnStmt(Stmt* stmt);
        bool TravelIntExpr(Stmt* stmt);
        virtual bool Visit(Stmt*);
};

#endif // _VISITOR_H_
