#include <stdio.h>
#include "ast.h"
#include "Expr.h"
#include "Stmt.h"
#include "lex.h"
#include "parser.h"
int main(int argc, char const* argv[]) {
    yyparse();
    return 0;
}
