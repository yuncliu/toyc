#ifndef FRONTEND_H_
#define FRONTEND_H_

#include "Stmt.h"

#include <memory>

class FrontEnd {
public:
    FrontEnd(){
    }
    virtual ~FrontEnd(){
    }
    virtual std::shared_ptr<Stmt> parse(std::string fileName) = 0;
};

#endif // FRONTEND_H_
