#ifndef _FLEXBISONFRONTEND_H__H_
#define _FLEXBISONFRONTEND_H_

#include "FrontEnd.h"
class FlexBisonFrontEnd: public FrontEnd {
public:
    FlexBisonFrontEnd();
    ~FlexBisonFrontEnd();
    virtual std::shared_ptr<Stmt> parse(std::string fileName);
};

#endif // ifndef _FLEXBISONFRONTEND_H_
