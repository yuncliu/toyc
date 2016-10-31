#ifndef _FLEXBISONFRONTEND_H__H_
#define _FLEXBISONFRONTEND_H_

#include "FrontEnd.h"
class FlexBisonFrontEnd: public FrontEnd {
public:
    FlexBisonFrontEnd();
    ~FlexBisonFrontEnd();
    virtual std::shared_ptr<ASTNode> parse(std::string fileName);
    static std::shared_ptr<ASTNode> ast;
private:
    std::shared_ptr<ASTNode> ast_beautify(std::shared_ptr<ASTNode> ast);
};

#endif // ifndef _FLEXBISONFRONTEND_H_
