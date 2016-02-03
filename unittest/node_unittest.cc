#include "gtest/gtest.h"
#include "node.h"
#include "lex.h"
#include "parser.h"

TEST(node, add) {
    Node a(1);
    Node b(1);
    Node c(0);
    c = a + b;
    EXPECT_EQ(c.value, 2);
}

TEST(node, add1) {
    Node a(1);
    Node b(1);
    Node c = a + b;
    EXPECT_EQ(c.value, 2);
}
