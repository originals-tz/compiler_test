#ifndef COMPILER_2SCANNER_H
#define COMPILER_2SCANNER_H

#include <string_view>
#include <string>
#include <optional>
#include <iostream>
#include <vector>

namespace test2
{
enum E_TOEKN {
    E_TOKEN_EOF = 1,
    E_TOKEN_PLUS,   //! +
    E_TOKEN_MINUS,  //! -
    E_TOKEN_STAR,   //! *
    E_TOKEN_SLASH,  //! /
    E_TOKEN_INILIT  //! integer
};

struct Token
{
    Token()
        : m_token(E_TOKEN_EOF)
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


        std::optional<Token> token;
        while ((token = Scan()))
        {
            m_token_vect.emplace_back(*token);
            if (token->m_token == E_TOKEN_EOF)
            {
                break;
            }
        }
        m_cur_token_index = 0;
        m_end_token_index = m_token_vect.size();
    }

    std::optional<Token> GetNextToken()
    {
        std::optional<Token> token;
        if (m_cur_token_index < m_end_token_index)
        {
            token = m_token_vect[m_cur_token_index];
            m_cur_token_index++;
        }
        return token;
    }

    void Rollback()
    {
        if (m_cur_token_index > 0)
        {
            m_cur_token_index--;
        }
    }

private:
    int GetInteger(char c)
    {
        int i = 0;
        while (In(c, m_integer_sv))
        {
            i = i * 10 + c - '0';
            c = Next();
        }
        if (c != EOF)
        {
            m_cur_index--;
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
                token.m_token = E_TOKEN_EOF;
                break;
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

    size_t m_cur_token_index;
    size_t m_end_token_index;
    size_t m_cur_index;
    size_t m_end_index;
    std::string_view m_input_sv;
    std::string_view m_skip_sv;
    std::string_view m_integer_sv;
    std::vector<Token> m_token_vect;
};

}
#endif  // COMPILER_SCANNER_H
