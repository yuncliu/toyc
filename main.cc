#include <stdio.h>
#include "Expr.h"
#include "Stmt.h"
#include "lex.h"
#include "parser.h"
#include "Visitor.h"
#include "DumpVisitor.h"
#include "LLVMVisitor.h"
std::weak_ptr<Stmt> root;
int main(int argc, char const* argv[]) {
    yyparse();
    Visitor* v1 = new DumpVisitor();
    Visitor* v2 = new LLVMVisitor();
    root->Accept(v1);
    root->Accept(v2);
    delete v1;
    delete v2;
    return 0;
}
