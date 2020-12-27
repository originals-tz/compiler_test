#ifndef COMPILER_1TEST_H
#define COMPILER_1TEST_H
#include "scanner.h"

namespace test1
{
void Test1(const std::string& input)
{
    std::cout << "[Test1], input = " << input << std::endl;
    Scanner scanner(input);
    std::optional<Token> token;
    while ((token = scanner.Scan()))
    {
        std::cout << "token : " << token->m_token << std::endl;
        if (token->m_intevalue != -1)
        {
            std::cout << "value : " << token->m_intevalue << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << "[Finish Test]\n" << std::endl;
}

void Test1()
{
    Test1("1+3-49 +24-546");
    Test1("1+...");
    Test1("");
}
}  // namespace test1

#endif  // COMPILER_1TEST_H
