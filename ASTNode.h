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
        TYPE_SPECIFIER,
        IDENTIFIER,
        CONSTANT,
        STATEMENT,
        IntegerLiteral,
        FloatingLiteral,
        FunctionDecl,
        VarDecl,
        TranslationUnitDecl
    }NodeType;
    ASTNode();
    ASTNode(std::string v, NodeType ty);
    virtual ~ASTNode();

    /**
     * @brief get basic information of this node;
     */
    virtual std::string info();
    std::vector<std::shared_ptr<ASTNode> > children;
    std::string value;
    NodeType type;
};

#if 1
/**
 * @class VarDecl
 * @brief Declaraton of a variable
 */
class VarDecl: public ASTNode {
    public:
        VarDecl();
        virtual ~VarDecl();
        virtual std::string info();
        std::string identifier;
        std::string type_specifier;
};
#endif

#endif /* ifndef ASTNODE_H_ */
