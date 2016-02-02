#include "gtest/gtest.h"
#include "node.h"
#include "lex.h"
#include "parser.h"

TEST(lex, first) {
    yy_scan_string("1+1;");
    EXPECT_EQ(yylex(), NUM);
    printf("%s\n", yytext);
    EXPECT_EQ(yylex(), '+');
    printf("%s\n", yytext);
    EXPECT_EQ(yylex(), NUM);
    printf("%s\n", yytext);
}

TEST(lex, yacc) {
    yy_scan_string("1+1;");
    EXPECT_EQ(yyparse(), 0);
}

TEST(lex, print) {
    yy_scan_string("print(1+1);\n");
    yy_scan_string("a=10;\n"
                   "print(a);\n");
    EXPECT_EQ(yyparse(), 0);
}
