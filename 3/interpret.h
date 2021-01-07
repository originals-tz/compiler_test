#ifndef COMPILER_3INTERP_H
#define COMPILER_3INTERP_H

#include <functional>

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

        std::function<void()> cal = [&](){
          left_value = right_value = 0;
          if (root->m_left)
          {
              left_value = Interpret(root->m_left);
          }
          if (root->m_right)
          {
              right_value = Interpret(root->m_right);
          }
        };

        switch (root->m_op)
        {
            case E_ASTOP_ADD:
                cal();
                return left_value + right_value;
            case E_ASTOP_SUBTRACT:
                cal();
                return left_value - right_value;
            case E_ASTOP_MULTIPLY:
                cal();
                return left_value * right_value;
            case E_ASTOP_DIVIDE:
                cal();
                return left_value / right_value;
            case E_ASTOP_INTLIT:
                return root->m_intvalue;
            case E_ASTOP_PRINT:
                // 打印
                if (root->m_right)
                {
                    cal();
                    std::cout << "[OUTPUT] : " << right_value << std::endl;
                }
                return 0;
            case E_ASTOP_EQUALS:
                // 赋值
                return 0;
            default:
                // 变量声明
                switch (root->m_op)
                {
                    case E_ASTOP_INT:
                        return 0;
                }
                break;
        }
        std::cout << "error : invalid op" << ASTOPInfo::GetInfo(root->m_op) << std::endl;
        exit(-1);
    }
};

Assembly InterpretAST::assembly;

}

#endif  // COMPILER_INTERP_H
