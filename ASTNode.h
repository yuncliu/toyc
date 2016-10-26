#ifndef ASTNODE_H_
#define ASTNODE_H_

#include <vector>
#include <memory>


class ASTNode {
public:
    typedef enum {
        DECL,
        STMT,
        EXPR,
        OP,
        INTEGER,
        DOUBLE,
        TYPE_SPECIFIER
    }NodeType;
    ASTNode();
    ~ASTNode();
    std::vector<std::shared_ptr<ASTNode> > children;
    std::string value;
    NodeType type;
};

#endif /* ifndef ASTNODE_H_ */
