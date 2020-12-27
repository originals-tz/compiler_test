#ifndef COMPILER_1SCANNER_H
#define COMPILER_1SCANNER_H

#include <cstring>
#include <iostream>
#include <optional>

namespace test1
{
enum E_TOEKN {
    E_TOKEN_BEGIN = 1,
    E_TOKEN_PLUS,   //! +
    E_TOKEN_MINUS,  //! -
    E_TOKEN_STAR,   //! *
    E_TOKEN_SLASH,  //! /
    E_TOKEN_INILIT  //! integer
};

struct Token
{
    Token()
        : m_token(E_TOKEN_BEGIN)
        , m_intevalue(-1)
    {
    }

    E_TOEKN m_token;
    int m_intevalue;
};

class Scanner
{
public:
    explicit Scanner(const std::string& input)
        : m_input_sv(input)
        , m_skip_sv(" \t\n\r\f")
        , m_integer_sv("0123456789")
    {
        m_cur_index = 0;
        m_end_index = m_input_sv.size();
    }

    int GetInteger(char c)
    {
        int i = 0;
        while (In(c, m_integer_sv))
        {
            i = i * 10 + c - '0';
            c = Next();
        }
        return i;
    }

    bool In(char c, std::string_view sv) { return sv.find(c) != std::string_view::npos; }

    char Skip()
    {
        char c = Next();
        while (In(c, m_skip_sv))
        {
            c = Next();
        }
        return c;
    }

    char Next()
    {
        char c = EOF;
        if (m_cur_index < m_end_index)
        {
            c = m_input_sv[m_cur_index];
            m_cur_index++;
        }
        return c;
    }

    std::optional<Token> Scan()
    {
        Token token;
        char c = Skip();
        switch (c)
        {
            case EOF:
                return {};
            case '+':
                token.m_token = E_TOKEN_PLUS;
                break;
            case '-':
                token.m_token = E_TOKEN_MINUS;
                break;
            case '*':
                token.m_token = E_TOKEN_STAR;
                break;
            case '/':
                token.m_token = E_TOKEN_SLASH;
                break;
            default:
                if (isdigit(c))
                {
                    token.m_intevalue = GetInteger(c);
                    token.m_token = E_TOKEN_INILIT;
                    break;
                }
                std::cout << "error, wrong token [" << c << "]" << std::endl;
                return {};
        }
        return token;
    }

private:
    size_t m_cur_index;
    size_t m_end_index;
    std::string_view m_input_sv;
    std::string_view m_skip_sv;
    std::string_view m_integer_sv;
};
}  // namespace test1
#endif  // COMPILER_1SCANNER_H
