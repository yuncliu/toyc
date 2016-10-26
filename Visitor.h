#ifndef _VISITOR_H_
#define _VISITOR_H_
#include <map>
#include <memory>
class ASTNode;

class Visitor {
    public:
        Visitor();
        virtual ~Visitor();
        virtual bool Visit(std::shared_ptr<ASTNode> s);
};

#endif // _VISITOR_H_
