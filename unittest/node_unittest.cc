#include "gtest/gtest.h"
#include "node.h"
#include "lex.h"
#include "parser.h"

TEST(node, add) {
    Node a;
    Node b;
    Node c;
    a.value = 1;
    b.value = 1;
    vector<Node> params;
    params.push_back(a);
    params.push_back(b);
    c.value = 0;
    c.type = Node::OPERATION;
    c.ex(Node::opADD, params);
    EXPECT_EQ(c.value, 2);
}
