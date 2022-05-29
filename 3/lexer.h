#ifndef COMPILER_3LEXER_H
#define COMPILER_3LEXER_H

#include "scanner.h"
#include <algorithm>
#include <memory>
#include <map>

namespace test3
{

enum E_ASTOP
{
    E_ASTOP_EOF = 0,
    E_ASTOP_ADD = 1,
    E_ASTOP_SUBTRACT = 2,
    E_ASTOP_MULTIPLY = 3,
    E_ASTOP_DIVIDE = 4,
    E_ASTOP_INTLIT = 5, E_ASTOP_ASSIGN = 6,
    E_ASTOP_IDENT = 7,
    E_ASTOP_PRINT = 8,
    E_ASTOP_INT = 9,
    E_ASTOP_SEMIT = 10,
    E_ASTOP_EQ, // ==
    E_ASTOP_NE, // !=
    E_ASTOP_LT, // <
    E_ASTOP_GT, // >
    E_ASTOP_LE, // <=
    E_ASTOP_GE, // >=
    E_ASTOP_IF, // if
    E_ASTOP_ELSE // else
};

struct ASTOPInfo
{
#define Case(x) case x: return #x;
    static std::string GetInfo(E_ASTOP& op)
    {
        switch (op)
        {
            Case(E_ASTOP_EOF)
            Case(E_ASTOP_ADD)
            Case(E_ASTOP_SUBTRACT)
            Case(E_ASTOP_MULTIPLY)
            Case(E_ASTOP_DIVIDE)
            Case(E_ASTOP_INTLIT)
            Case(E_ASTOP_ASSIGN)
            Case(E_ASTOP_IDENT)
            Case(E_ASTOP_PRINT)
            Case(E_ASTOP_INT)
            Case(E_ASTOP_SEMIT)
            Case(E_ASTOP_EQ)
            Case(E_ASTOP_NE)
            Case(E_ASTOP_LT)
            Case(E_ASTOP_GT)
            Case(E_ASTOP_LE)
            Case(E_ASTOP_GE)
        }
        return "";
    }
};


struct ASTnode
{
    enum E_ASTOP m_op = E_ASTOP_EOF;
    std::shared_ptr<ASTnode> m_left;
    std::shared_ptr<ASTnode> m_right;
    int m_intvalue = -1;
    std::string m_str;

    static std::shared_ptr<ASTnode> MakeAstNode(Token& t, E_ASTOP op)
    {
        auto node = std::make_shared<ASTnode>();
        node->m_op = op;
        node->m_left = nullptr;
        node->m_right = nullptr;
        node->m_intvalue = t.m_intevalue;
        node->m_str = t.m_str;
        return node;
    }

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

struct ParseResult
{
    std::vector<ASTnode> m_nodes;
    std::map<std::string, Token> m_sym_table;
};

class Lexer
{
public:
    explicit Lexer(std::shared_ptr<Scanner> scanner)
        : m_scanner(std::move(scanner))
    {
    }

    bool GetStatement(std::optional<Token> cur, std::vector<ASTNodePtr>& nodes_vect)
    {
        switch (cur->m_token)
        {
            case E_TOKEN_IDENT: {
                nodes_vect.emplace_back(AssignStatement());
                break;
            }
            case E_TOKEN_PRINT: {
                nodes_vect.emplace_back(PrintStatement());
                break;
            }
            case E_TOKEN_INT: {
                nodes_vect.emplace_back(VarDeclaration());
                break;
            }
            case E_TOKEN_EOF: {
                m_scanner->GetNextToken();
                break;
            }
            case E_TOKEN_IF: {
            }
            default:
                return false;
        }
        return true;
    }

    std::vector<ASTNodePtr> Parse()
    {
        std::optional<Token> cur;
        std::vector<ASTNodePtr> nodes_vect;
        while ((cur = m_scanner->ViewCurToken()))
        {
            if (!GetStatement(cur, nodes_vect))
            {
                break ;
            }
        }
        return nodes_vect;
    }

    E_ASTOP Match(E_TOKEN type)
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

    ASTNodePtr IFStatement()
    {
        auto root = Match(E_TOKEN_IF);
        Match(E_TOKEN_LPAREN);
        auto left = CompareExpr();
        Match(E_TOKEN_RPAREN);
        return nullptr;
    }

    ASTNodePtr VarDeclaration()
    {
        auto root = Match(E_TOKEN_INT);
        auto left = m_scanner->GetNextToken();
        assert(left->m_token == E_TOKEN_IDENT);
        Match(E_TOKEN_SEMIT);
        auto left_node = ASTnode::MakeAstNode(*left, Trans(left->m_token));
        m_sym_table[left->m_str] = *left;
        return ASTnode::MakeAstNode(root, left_node, nullptr);
    }

    ASTNodePtr PrintStatement()
    {
        auto root_op = Match(E_TOKEN_PRINT);
        auto right = AdditiveExpr();
        Match(E_TOKEN_SEMIT);
        return ASTnode::MakeAstNode(root_op, nullptr, right);
    }

    ASTNodePtr AssignStatement()
    {
        auto id = m_scanner->GetNextToken();
        auto right_node = ASTnode::MakeAstNode(*id, Trans(id->m_token));
        Match(E_TOKEN_EQUALS);
        auto left_node = AdditiveExpr();
        Match(E_TOKEN_SEMIT);
        return ASTnode::MakeAstNode(E_ASTOP_ASSIGN, left_node, right_node);
    }

    static E_ASTOP Trans(E_TOKEN token_type)
    {
        switch (token_type)
        {
            case E_TOKEN_EOF:
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
                return E_ASTOP_ASSIGN;
            case E_TOKEN_IDENT:
                return E_ASTOP_IDENT;
            case E_TOKEN_PRINT:
                return E_ASTOP_PRINT;
            case E_TOKEN_INT:
                return E_ASTOP_INT;
            case E_TOKEN_SEMIT:
                return E_ASTOP_SEMIT;
            case E_TOKEN_EQ:
                return E_ASTOP_EQ;
            case E_TOKEN_NE:
                return E_ASTOP_NE;
            case E_TOKEN_LT:
                return E_ASTOP_LT;
            case E_TOKEN_GT:
                return E_ASTOP_GT;
            case E_TOKEN_LE:
                return E_ASTOP_LE;
            case E_TOKEN_GE:
                return E_ASTOP_GE;
            case E_TOKEN_IF:
                return E_ASTOP_IF;
            case E_TOKEN_ELSE:
                return E_ASTOP_ELSE;
            default:
                assert(false);
                break;
        };
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
        if (token->m_token == E_TOKEN_IDENT)
        {
            auto node = ASTnode::MakeAstNode(Trans(token->m_token), nullptr, nullptr, 0);
            node->m_str = token->m_str;
            return node;
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

    template <typename T>
    bool In(const T& x, const std::vector<T>& vect)
    {
        return std::any_of(vect.begin(), vect.end(), [&](const T& t){ return t == x;});
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
        m_scanner->Rollback();
        return root;
    }

    ASTNodePtr CompareExpr()
    {
        auto root = Primary();
        ASTNodePtr op;
        while ((op = Operator()) &&
               In(op->m_op, { E_ASTOP_EQ,
                            E_ASTOP_NE,
                            E_ASTOP_LT,
                            E_ASTOP_GT,
                            E_ASTOP_LE,
                            E_ASTOP_GE }))
        {
            auto left = std::move(root);
            auto right = Primary();
            root = ASTnode::MakeAstNode(op->m_op, left, right);
        }
        m_scanner->Rollback();
        return root;
    }

    ASTNodePtr MultiplicativeExpr()
    {
        auto root = CompareExpr();
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
    std::map<std::string, Token> m_sym_table;
};

}

#endif  // COMPILER_LEXER_H