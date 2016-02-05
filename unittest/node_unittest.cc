#include "gtest/gtest.h"
#include "node.h"
#include "lex.h"
#include "parser.h"

TEST(node, print) {
    yy_scan_string("print(1+1);\n");
    EXPECT_EQ(yyparse(), 0);
}
