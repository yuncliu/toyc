#include <stdio.h>
#include <iostream>
#include <fstream>

#include "Expr.h"
#include "Stmt.h"
#include "Visitor.h"
#include "DumpVisitor.h"
#include "LLVMVisitor.h"
#include "llvm/Bitcode/ReaderWriter.h"

#include "FlexBisonFrontEnd.h"

int main(int argc, char const* argv[]) {

    if (argc < 2) {
        std::cout <<"fatal error: no input files"<<std::endl;
        return 0;
    }

    std::string inputFile(argv[1]);
    std::string inputFileNoExt = inputFile.substr(0, inputFile.find("."));
    std::cout << inputFileNoExt << std::endl;

    FlexBisonFrontEnd frontend;
    std::shared_ptr<Stmt> ast = frontend.parse(inputFile);

    std::shared_ptr<DumpVisitor> v1(new DumpVisitor());
    v1->Visit(ast);

    // generate LLVM IR
    std::shared_ptr<LLVMVisitor> v2(new LLVMVisitor());
    v2->Visit(ast);
    std::unique_ptr<Module> module = v2->getModule();

    // write IR in to file
    llvm::StringRef sRefName(inputFileNoExt + ".ll");
    std::error_code  err_code;
    llvm::raw_fd_ostream raw(sRefName, err_code,  (llvm::sys::fs::OpenFlags)8);
    module->print(raw, NULL);

    return 0;
}
