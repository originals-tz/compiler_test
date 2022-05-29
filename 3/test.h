#ifndef COMPILER_3TEST_H
#define COMPILER_3TEST_H

#include <cassert>
#include <fstream>
#include "interpret.h"

namespace test3
{

void TestScan(const std::string& input)
{
    std::cout << "[Test] input = " << input << std::endl;
    auto scanner = std::make_shared<Scanner>(input);
    auto vect = scanner->GetToken();
    for (auto& tk : vect)
    {
        std::cout << E_TOKEN_NAME[tk.m_token] << std::endl;
    }
}

std::vector<ASTNodePtr> Test3(const std::string& input)
{
    std::cout << "[Test] input = " << input << std::endl;
    auto scanner = std::make_shared<Scanner>(input);
    Lexer lexer(scanner);
    auto res = lexer.Parse();
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
    std::string input;
//    input =
//    R"(
//    print 1+2+3;
//    print 2+3-4;
//    int s;
//    s = 100;
//    print s;
//    print s == 100;
//    print s < 100;
//    print s >= 100;
//    print s + 100;
//    if (s + 2 > 10) {
//        print s + 1000;
//    }
//    print 1 + 2 * s;)";
    input = "if (1 +1 > 2) {print 100;}";
    TestScan(input);
//    auto data = Test3(input);
//    for (auto& node : data)
//    {
//        InterpretAST::Interpret(node);
//    }
}

}

#endif  // COMPILER_TEST_H
