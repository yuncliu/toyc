#include "gtest/gtest.h"
#include "lex.h"
#include "parser.h"

TEST(lex, first) {
    yy_scan_string("1+1");
    EXPECT_EQ(yylex(), NUM);
    printf("%s\n", yytext);
    EXPECT_EQ(yylex(), '+');
    printf("%s\n", yytext);
    EXPECT_EQ(yylex(), NUM);
    printf("%s\n", yytext);
}

TEST(lex, yacc) {
    yy_scan_string("1+1\n");
    EXPECT_EQ(yyparse(), 0);
}
