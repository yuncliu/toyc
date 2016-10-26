#ifndef FRONTEND_H_
#define FRONTEND_H_

#include <memory>
#include "ASTNode.h"

class FrontEnd {
public:
    FrontEnd(){
    }
    virtual ~FrontEnd(){
    }
    virtual std::shared_ptr<ASTNode> parse(std::string fileName) = 0;
};

#endif // FRONTEND_H_
