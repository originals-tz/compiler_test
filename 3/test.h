#ifndef COMPILER_3TEST_H
#define COMPILER_3TEST_H

#include <cassert>
#include <fstream>
#include "interpret.h"

namespace test3
{

ASTNodePtr Test2(const std::string& input)
{
    std::cout << "[Test2] input = " << input << std::endl;
    auto scanner = std::make_shared<Scanner>(input);
    Lexer lexer(scanner);
    auto res = lexer.Parse();
    if (res->m_op == E_ASTOP_EOF)
    {
        return nullptr;
    }
    return res;
}

void GenerateAssembly(ASTNodePtr res)
{
    static int i = 0;
    auto ret = InterpretAST::InterpretAssembly(res);
    std::string data = InterpretAST::assembly.Generate(ret);
    std::cout << data << std::endl;

    std::stringstream ss;
    ss << i << ".s";
    std::ofstream file(ss.str(), std::fstream::out | std::fstream::trunc);
    assert(file.is_open());
    file << data << std::endl;
    file.flush();
    i++;
}

void Test3()
{
    ASTNodePtr res;
    res = Test2("");
    assert(!res);
    res = Test2("1");

    res = Test2("1+2-3");
    std::cout << InterpretAST::Interpret(res) << std::endl;
    GenerateAssembly(res);

    res = Test2("1*3/3");
    std::cout << InterpretAST::Interpret(res) << std::endl;
    GenerateAssembly(res);

    res = Test2("1*3-2");
    std::cout << InterpretAST::Interpret(res) << std::endl;
    GenerateAssembly(res);

    res = Test2("1+3*2+2");
    std::cout << InterpretAST::Interpret(res) << std::endl;
    GenerateAssembly(res);
}

}

#endif  // COMPILER_TEST_H
