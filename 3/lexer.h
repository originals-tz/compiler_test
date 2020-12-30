#ifndef COMPILER_3LEXER_H
#define COMPILER_3LEXER_H

#include "scanner.h"
#include <memory>

namespace test3
{

enum E_ASTOP
{
    E_ASTOP_EOF,
    E_ASTOP_ADD,
    E_ASTOP_SUBTRACT,
    E_ASTOP_MULTIPLY,
    E_ASTOP_DIVIDE,
    E_ASTOP_INTLIT,
    E_ASTOP_EQUALS,
    E_ASTOP_IDENT,
    E_ASTOP_PRINT,
    E_ASTOP_INT,
    E_ASTOP_SEMIT
};

struct ASTnode
{
    enum E_ASTOP m_op = E_ASTOP_EOF;
    std::shared_ptr<ASTnode> m_left;
    std::shared_ptr<ASTnode> m_right;
    int m_intvalue = -1;

    static std::shared_ptr<ASTnode> MakeAstNode(E_ASTOP op, std::shared_ptr<ASTnode> left, std::shared_ptr<ASTnode> right, int value = -1)
    {
        auto node = std::make_shared<ASTnode>();
        node->m_op = op;
        node->m_left = left;
        node->m_right = right;
        node->m_intvalue = value;
        return node;
    }
};

using ASTNodePtr = std::shared_ptr<ASTnode>;

class Lexer
{
public:
    explicit Lexer(std::shared_ptr<Scanner>  scanner)
        : m_scanner(std::move(scanner))
    {
    }

    ASTNodePtr Parse()
    {
        return AdditiveExpr();
    }

    E_ASTOP Match(E_TOEKN type)
    {
        auto token = m_scanner->GetNextToken();
        if (!token)
        {
            std::cout << "match error : nullptr" << std::endl;
            exit(-1);
        }
        if (token->m_token == type)
        {
            return Trans(type);
        }

        std::cout << "match error : expect " << type << ", get " << token->m_token << std::endl;
        exit(-1);
    }

    void PrintStatement()
    {
        auto root_op = Match(E_TOKEN_PRINT);
        auto right = AdditiveExpr();
        ASTnode::MakeAstNode(root_op, nullptr, right);
    }

    static E_ASTOP Trans(E_TOEKN token_type)
    {
        switch (token_type)
        {
            case E_TOKEN_EOF :
                return E_ASTOP_EOF;
            case E_TOKEN_PLUS:
                return E_ASTOP_ADD;
            case E_TOKEN_MINUS:
                return E_ASTOP_SUBTRACT;
            case E_TOKEN_STAR:
                return E_ASTOP_MULTIPLY;
            case E_TOKEN_SLASH:
                return E_ASTOP_DIVIDE;
            case E_TOKEN_INILIT:
                return E_ASTOP_INTLIT;
            case E_TOKEN_EQUALS:
                return E_ASTOP_EQUALS;
            case E_TOKEN_IDENT:
                return E_ASTOP_IDENT;
            case E_TOKEN_PRINT:
                return E_ASTOP_PRINT;
            case E_TOKEN_INT:
                return E_ASTOP_INT;
            case E_TOKEN_SEMIT:
                return E_ASTOP_SEMIT;
        }
        return E_ASTOP_EOF;
    }

    ASTNodePtr Primary()
    {
        auto token = m_scanner->GetNextToken();
        if (!token)
        {
            std::cout << "error : no primary" << std::endl;
            exit(-1);
        }
        return ASTnode::MakeAstNode(Trans(token->m_token), nullptr, nullptr, token->m_intevalue);
    }

    ASTNodePtr Operator()
    {
        auto token = m_scanner->GetNextToken();
        if (!token)
        {
            return nullptr;
        }
        return ASTnode::MakeAstNode(Trans(token->m_token), nullptr, nullptr);
    }

    ASTNodePtr AdditiveExpr()
    {
        auto root = MultiplicativeExpr();
        ASTNodePtr op;
        while ((op = Operator()) && (op->m_op == E_ASTOP_ADD || op->m_op == E_ASTOP_SUBTRACT))
        {
            auto left = std::move(root);
            auto right = MultiplicativeExpr();
            root = ASTnode::MakeAstNode(op->m_op, left, right);
        }
        return root;
    }

    ASTNodePtr MultiplicativeExpr()
    {
        auto root = Primary();
        ASTNodePtr op;
        while ((op = Operator()) && (op->m_op == E_ASTOP_MULTIPLY || op->m_op == E_ASTOP_DIVIDE))
        {
            auto left = std::move(root);
            auto right = Primary();
            root = ASTnode::MakeAstNode(op->m_op, left, right);
        }
        m_scanner->Rollback();
        return root;
    }

private:
    std::shared_ptr<Scanner> m_scanner;
};

}

#endif  // COMPILER_LEXER_H
