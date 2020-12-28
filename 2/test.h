#ifndef COMPILER_2TEST_H
#define COMPILER_2TEST_H

#include <cassert>
#include "interpret.h"

namespace test2
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

void Test2()
{
    ASTNodePtr res;
    res = Test2("");
    assert(!res);
    res = Test2("1");
    res = Test2("1+2-3");

    auto ret = InterpretAST::InterpretAssembly(res);
    std::string data = InterpretAST::assembly.Generate(ret);
    std::cout << data << std::endl;

    std::cout << InterpretAST::Interpret(res) << std::endl;

    res = Test2("1*3/3");
    std::cout << InterpretAST::Interpret(res) << std::endl;

    res = Test2("1*3-2");
    std::cout << InterpretAST::Interpret(res) << std::endl;

    res = Test2("1+3*2+2");
    std::cout << InterpretAST::Interpret(res) << std::endl;
}

}

#endif  // COMPILER_TEST_H
