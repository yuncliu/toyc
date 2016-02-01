#include "gtest/gtest.h"
#include "lex.h"
#include "parser.h"

TEST(lex, first) {
    yy_scan_string("1+1");
    EXPECT_EQ(yylex(), NUM);
    EXPECT_EQ(1,1);
}
