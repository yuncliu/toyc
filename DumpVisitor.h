#ifndef _DUMP_VISITOR_H_
#define _DUMP_VISITOR_H_
#include <map>
#include <vector>
#include "Visitor.h"
#define LIGHT_GREEN "\x1B[0;1;32m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_RESET "\033[0m"
class DumpVisitor;
class ASTNode;

class DumpVisitor:public Visitor {
    protected:
        std::vector<std::string> prefix;
        bool is_last;
    public:
        DumpVisitor();
        virtual ~DumpVisitor();
        virtual bool Visit(std::shared_ptr<ASTNode> s);
        void print_prefix();
};

#endif // _VISITOR_H_
