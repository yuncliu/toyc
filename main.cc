#include <stdio.h>
#include "node.h"
#include "lex.h"
#include "parser.h"

int main(int argc, char const* argv[]) {
    yyparse();
    return 0;
}
