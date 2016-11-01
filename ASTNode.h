#ifndef ASTNODE_H_
#define ASTNODE_H_

#include <vector>
#include <memory>

#ifdef __APPLE__
#include <istream>
#endif


class ASTNode {
public:
    typedef enum {
        INIT_DECLARATOR = 1,
        DECL            = 2,
        STMT            = 3,
        EXPR            = 4,
        OP              = 5,
        INTEGER         = 6,
        DOUBLE          = 7,
        TYPE_SPECIFIER  = 8,
        IDENTIFIER      = 9,
        CONSTANT        = 10,
        STATEMENT       = 11,
        IntegerLiteral  = 12,
        FloatingLiteral = 13,
        FunctionDecl    = 14,
        VarDecl         = 15,
        ParmVarDecl     = 16,
        BinaryOperator  = 17,
        TranslationUnitDecl = 18
    }NodeType;
    ASTNode();
    ASTNode(std::string v, NodeType ty);
    virtual ~ASTNode();

    /**
     * @brief get basic information of this node;
     */
    virtual std::string info();

    /*
     * @brief initialize from a ASTNode, mainly used by subclasses
     */
    virtual void initialize(ASTNode* _node);

    template< class _Tp>
    std::shared_ptr<_Tp> get() {
        std::shared_ptr<_Tp> p(new _Tp);
        if (p->type == this->type) {
            p->initialize(this);
            return p;
        }else {
            return std::shared_ptr<_Tp>(NULL);
        }
    }

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

        /**
         * @brief initilaize from orginal ASTNode
         */
        virtual void initialize(ASTNode* _node);
};
#endif

class IntegerLiteral: public ASTNode {
    public:
        IntegerLiteral();
        virtual ~IntegerLiteral();
        virtual std::string info();

        /**
         * @brief initilaize from orginal ASTNode
         */
        virtual void initialize(ASTNode* _node);
};

class FloatingLiteral: public ASTNode {
    public:
        FloatingLiteral();
        virtual ~FloatingLiteral();
        virtual std::string info();
        /**
         * @brief initilaize from orginal ASTNode
         */
        virtual void initialize(ASTNode* _node);
};

class BinaryOperator: public ASTNode {
    public:
        BinaryOperator();
        ~BinaryOperator();
        virtual std::string info();
        /**
         * @brief initilaize from orginal ASTNode
         */
        virtual void initialize(ASTNode* _node);

        /**
         * @brief operationa eg: "+" "-"
         */
        std::string op;
};

class Identifier: public ASTNode {
    public:
        Identifier();
        ~Identifier();
        virtual std::string info();
        virtual void initialize(ASTNode* _node);
};

class FunctionDecl: public ASTNode {
    public:
        FunctionDecl();
        ~FunctionDecl();
        virtual std::string info();
        std::string return_ty;
        std::string identifier;
};

class ParmVarDecl: public ASTNode {
    public:
        ParmVarDecl();
        ~ParmVarDecl();
        virtual std::string info();
        std::string ty;
        std::string identifier;
};

#endif /* ifndef ASTNODE_H_ */
