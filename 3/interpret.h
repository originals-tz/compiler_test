#ifndef COMPILER_3INTERP_H
#define COMPILER_3INTERP_H

#include "lexer.h"
#include "assembly.h"

namespace test3
{

struct InterpretAST
{
    static Assembly assembly;

    static int InterpretAssembly(ASTNodePtr root)
    {
        if (!root)
        {
            std::cout << "nullptr" << std::endl;
            exit(-1);
        }
        int left_value, right_value;
        left_value = right_value = 0;
        if (root->m_left)
        {
            left_value = InterpretAssembly(root->m_left);
        }
        if (root->m_right)
        {
            right_value = InterpretAssembly(root->m_right);
        }

        switch (root->m_op)
        {
            case E_ASTOP_ADD:
                return assembly.Add(left_value, right_value);
            case E_ASTOP_SUBTRACT:
                return assembly.Sub(left_value, right_value);
            case E_ASTOP_MULTIPLY:
                return assembly.Mul(left_value, right_value);
            case E_ASTOP_DIVIDE:
                return assembly.Div(left_value, right_value);
            case E_ASTOP_INTLIT:
                return assembly.Load(root->m_intvalue);
            default:
                break;
        }
        std::cout << "error : invalid op" << std::endl;
        exit(-1);
    }

    static int Interpret(ASTNodePtr root)
    {
        if (!root)
        {
            std::cout << "nullptr" << std::endl;
            exit(-1);
        }
        int left_value, right_value;
        left_value = right_value = 0;
        if (root->m_left)
        {
            left_value = Interpret(root->m_left);
        }
        if (root->m_right)
        {
            right_value = Interpret(root->m_right);
        }

        switch (root->m_op)
        {
            case E_ASTOP_ADD:
                return left_value + right_value;
            case E_ASTOP_SUBTRACT:
                return left_value - right_value;
            case E_ASTOP_MULTIPLY:
                return left_value * right_value;
            case E_ASTOP_DIVIDE:
                return left_value / right_value;
            case E_ASTOP_INTLIT:
                return root->m_intvalue;
            default:
                break;
        }
        std::cout << "error : invalid op" << std::endl;
        exit(-1);
    }
};

Assembly InterpretAST::assembly;

}

#endif  // COMPILER_INTERP_H
