#include <stdio.h>
#include "lex.h"
#include "parser.h"

int main(int argc, char const* argv[]) {
    yy_scan_string("1+1");
    while(yylex()) {
    }
    return 0;
}
