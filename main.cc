#include <stdio.h>
#include "Expr.h"
#include "Stmt.h"
#include "lex.h"
#include "parser.h"
#include "Visitor.h"
#include "DumpVisitor.h"
#include "LLVMVisitor.h"
std::shared_ptr<Stmt> root(NULL);
int main(int argc, char const* argv[]) {
    yyparse();
    Visitor* v1 = new DumpVisitor();
    v1->Visit(root);
    delete v1;
    Visitor* v2 = new LLVMVisitor();
    v2->Visit(root);
    delete v2;
    return 0;
}
