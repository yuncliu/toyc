#ifndef _VISITOR_H_
#define _VISITOR_H_
#include <map>
#include <memory>
class Stmt;

class Visitor {
    public:
        Visitor();
        virtual ~Visitor();
        virtual bool Visit(std::shared_ptr<Stmt> s);
};

#endif // _VISITOR_H_
