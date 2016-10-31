#include "DumpVisitor.h"
#include "ASTNode.h"
#include <iostream>

DumpVisitor::DumpVisitor():is_last(false) {
}

DumpVisitor::~DumpVisitor() {
}

bool DumpVisitor::Visit(std::shared_ptr<ASTNode> s) {
    if (!s) {
        return false;
    }
    //std::string a = s->getSelfName();
    //VISIT_FUNC visit_func = this->getFunction(a);
    //if (NULL == visit_func) {
    //    printf("No visit function for [%s]\n", s->getSelfName().c_str());
    //    return false;
    //}
    print_prefix();
    //printf("%s\n", s->value.c_str());
    std::cout << s->info() <<std::endl;
    is_last = false;
    int size = s->children.size();
    if (size != 0) {
        this->prefix.push_back("|-");
    }
    for (int i = 0; i < size; ++i){
        if (i == size - 1) {
            is_last = true;
        }
        this->Visit(s->children[i]);
    }
    if (size != 0) {
        this->prefix.pop_back();
    }
    return true;
}
/**
 *print out prefix,
 *ConmpoundStmt:
 *`-Function: name [main] return type [int]   prefix="`-"
 *  |-FunctionProtoType                       prefix="  |-"
 *  | |-ID:[main]                             prefix="  | |-"
 *  | |-TypeExpr: [int]
 *  | `-No Parameters
 *  `-ConmpoundStmt:
 *    `-ReturnStmt:
 *      `-IntExpr Value = [0]
 */
void DumpVisitor::print_prefix() {
    printf(COLOR_BLUE); //Blue
    int size = prefix.size();
    for (int i = 0; i < size; ++i) {
        std::string t = prefix[i];
        if (t == "|-" && i != size-1) {
            t = "| ";
        }
        if (is_last && i == size-1) {
            t = "`-";
            prefix[i] = "  ";
        }
        printf("%s", t.c_str());
    }
    printf(COLOR_RESET);
}
