#ifndef COMPILER_2INTERP_H
#define COMPILER_2INTERP_H

#include "lexer.h"

namespace test2
{

struct InterpretAST
{
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
           default:
               return root->m_intvalue;
       }
   }
};
}

#endif  // COMPILER_INTERP_H
