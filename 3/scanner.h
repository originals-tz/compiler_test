#ifndef COMPILER_3SCANNER_H
#define COMPILER_3SCANNER_H

#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace test3
{
#define ENUM                  \
    X(E_TOKEN_EOF=0, "eof")       \
    X(E_TOKEN_PLUS, "(+)plus")     \
    X(E_TOKEN_MINUS, "(-)minus")   \
    X(E_TOKEN_STAR, "(*)star")     \
    X(E_TOKEN_SLASH, "(/)slash")   \
    X(E_TOKEN_INILIT, "(integer)inilit") \
    X(E_TOKEN_EQUALS, "(=)equals") \
    X(E_TOKEN_IDENT, "var")     \
    X(E_TOKEN_PRINT, "print")   \
    X(E_TOKEN_INT, "int")       \
    X(E_TOKEN_SEMIT, "(;)semit")   \
    X(E_TOKEN_EQ, "(==)eq")         \
    X(E_TOKEN_NE, "(!=)ne")         \
    X(E_TOKEN_LT, "(<)lt")         \
    X(E_TOKEN_GT, "(>)gt")         \
    X(E_TOKEN_LE, "(<=)le")         \
    X(E_TOKEN_GE, "(>=)ge")         \
    X(E_TOKEN_LBRACE, "{ : lbrace") \
    X(E_TOKEN_RBRACE, "} : rbrace") \
    X(E_TOKEN_LPAREN, "( : lparen") \
    X(E_TOKEN_RPAREN, ") : rparen") \
    X(E_TOKEN_IF, "if")         \
    X(E_TOKEN_ELSE, "else")

#define X(value, name) value,
enum E_TOKEN {
    ENUM
};
#undef X

#define X(value, name) name,
char const * E_TOKEN_NAME[] = {
    ENUM
};
#undef X

struct Token
{
    Token()
        : m_token(E_TOKEN_EOF)
        , m_intevalue(-1)
    {
    }

    E_TOKEN m_token;
    int m_intevalue;
    std::string m_str;
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
        m_last_index = 0;
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

    std::optional<Token> ViewCurToken()
    {
        std::optional<Token> token;
        if (m_cur_token_index < m_end_token_index)
        {
            token = m_token_vect[m_cur_token_index];
        }
        return token;
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

    std::vector<Token> GetToken() { return m_token_vect; }

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

    std::string GetWord(char c)
    {
        std::stringstream ss;
        while (isalpha(c) || c == '_')
        {
            ss << c;
            c = Next();
        }
        m_cur_index--;
        return ss.str();
    }

    static E_TOKEN GetTypeOfWord(const std::string& str)
    {
        if (str == "print")
        {
            return E_TOKEN_PRINT;
        }
        if (str == "int")
        {
            return E_TOKEN_INT;
        }
        if (str == "if")
        {
            return E_TOKEN_IF;
        }
        if (str == "else")
        {
            return E_TOKEN_ELSE;
        }
        return E_TOKEN_IDENT;
    }

    static bool In(char c, std::string_view sv) { return sv.find(c) != std::string_view::npos; }

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
        int tmp_last = m_cur_index;
        if (m_cur_index < m_end_index)
        {
            c = m_input_sv[m_cur_index];
            m_cur_index++;
        }
        if (m_cur_index != tmp_last)
        {
            m_last_index = tmp_last;
        }
        return c;
    }

    void Last() { m_cur_index = m_last_index; }

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
            case '=':
                if (Next() == '=')
                {
                    token.m_token = E_TOKEN_EQ;
                    break;
                }
                Last();
                token.m_token = E_TOKEN_EQUALS;
                break;
            case '!':
                c = Next();
                if (c == '=')
                {
                    token.m_token = E_TOKEN_NE;
                    break;
                }
                std::cout << "error, wrong token [" << c << "], excpet = [!=]" << std::endl;
                return {};
            case '>':
                c = Next();
                if (c == '=')
                {
                    token.m_token = E_TOKEN_GE;
                    break;
                }
                Last();
                token.m_token = E_TOKEN_GT;

                break;
            case '<':
                c = Next();
                if (c == '=')
                {
                    token.m_token = E_TOKEN_LE;
                    break;
                }
                Last();
                token.m_token = E_TOKEN_LT;
                break;
            case ';':
                token.m_token = E_TOKEN_SEMIT;
                break;
            case '(':
                token.m_token = E_TOKEN_LPAREN;
                break;
            case ')':
                token.m_token = E_TOKEN_RPAREN;
                break;
            case '{':
                token.m_token = E_TOKEN_LBRACE;
                break;
            case '}':
                token.m_token = E_TOKEN_RBRACE;
                break;
            default:
                if (isdigit(c))
                {
                    token.m_intevalue = GetInteger(c);
                    token.m_token = E_TOKEN_INILIT;
                    break;
                }
                if (isalpha(c) || c == '_')
                {
                    auto str = GetWord(c);
                    token.m_str = str;
                    token.m_token = GetTypeOfWord(str);
                    break;
                }
                std::cout << "error, wrong token [" << c << "]" << std::endl;
                return {};
        }
        return token;
    }

    size_t m_cur_token_index;
    size_t m_end_token_index;
    size_t m_last_index;
    size_t m_cur_index;
    size_t m_end_index;
    std::string_view m_input_sv;
    std::string_view m_skip_sv;
    std::string_view m_integer_sv;
    std::vector<Token> m_token_vect;
};

}  // namespace test3
#endif  // COMPILER_SCANNER_H
