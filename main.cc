#include <stdio.h>
#include <iostream>
#include <fstream>

#include "FlexBisonFrontEnd.h"
#include "DumpVisitor.h"

int main(int argc, char const* argv[]) {

    if (argc < 2) {
        std::cout <<"fatal error: no input files"<<std::endl;
        return 0;
    }

    std::string inputFile(argv[1]);
    std::string inputFileNoExt = inputFile.substr(0, inputFile.find("."));
    std::cout << "compiling: " << inputFile << std::endl;

    FlexBisonFrontEnd frontend;
    std::shared_ptr<ASTNode> ast = frontend.parse(inputFile);
    if (!ast) {
        printf("No AST tree!\n");
        return 0;
    }
    DumpVisitor d;
    d.Visit(ast);
    return 0;
}
