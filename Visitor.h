#ifndef _VISITOR_H_
#define _VISITOR_H_
#include <map>
class Stmt;

class Visitor {
    public:
        Visitor();
        virtual ~Visitor();
        virtual bool Visit(Stmt* s);
};

#endif // _VISITOR_H_
