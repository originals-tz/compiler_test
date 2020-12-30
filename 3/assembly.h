#ifndef COMPILER_3ASSEMBLY_H
#define COMPILER_3ASSEMBLY_H

#include <array>
#include <sstream>

namespace test3
{

class Assembly
{
public: 
    Assembly()
    {
        for (int i = 0; i < m_reg_label.size(); i++)
        {
            m_reg_label[i] = "%r" + std::to_string(8 + i);
        }
        Free();
    }

    void Print(int r)
    {
        m_ss << "\tmovq\t" << m_reg_label[r] << ", %rdi" << std::endl;
        m_ss << "\tcall\tprintint" << std::endl;
        Free(r);
    }

    void PostAmble()
    {
        m_ss << "\tmovl\t$0, %eax\n\tpopq\t%rbp\n\tret\n";
    }

    std::string Generate(int r)
    {
        Print(r);
        PostAmble();
        std::string tmp = m_ss.str();
        m_ss.str("");
        GenerateBase();
        m_ss << tmp;
        tmp = m_ss.str();
        m_ss.str("");
        m_ss.clear();
        return tmp;
    }

    int Load(int value)
    {
        int reg = Alloc();
        m_ss << "\tmovq\t$" << value << ", " << m_reg_label[reg] << std::endl;
        return reg;
    }
    
    int Add(int r1, int r2)
    {
        m_ss << "\taddq\t" << m_reg_label[r1] << ", " << m_reg_label[r2] << std::endl;
        Free(r1);
        return r2;
    }

    int Sub(int r1, int r2)
    {
        m_ss << "\tsubq\t" << m_reg_label[r1] << ", " << m_reg_label[r2] << std::endl;
        Free(r1);
        return r2;
    }

    int Mul(int r1, int r2)
    {
        m_ss << "\timulq\t" << m_reg_label[r1] << ", " << m_reg_label[r2] << std::endl;
        Free(r1);
        return r2;
    }

    int Div(int r1, int r2)
    {
        m_ss << "\tmovq\t" << m_reg_label[r1] << ", %rax" << std::endl;
        m_ss << "\tcqo" << std::endl;
        m_ss << "\tidivq\t" << m_reg_label[r2] << std::endl;
        m_ss << "\tmovq\t%rax, " << m_reg_label[r1] << std::endl;
        Free(r2);
        return r1;
    }
public:

    int Alloc()
    {
        for (int i = 0; i < m_reg.size(); i++)
        {
            if (m_reg[i])
            {
                m_reg[i] = 0;
                return i;
            }
        }
        assert(false && "alloc error");
    }

    void Free(int i)
    {
        m_reg[i] = 1;
    }

    void Free()
    {
        m_reg.fill(1);
    }

    void GenerateBase()
    {
        m_ss << 
        "\t.text\n"
        ".LC0:\n"
        "\t.string\t\"%d\\n\"\n"
        "printint:\n"
        "\tpushq\t%rbp\n"
        "\tmovq\t%rsp, %rbp\n"
        "\tsubq\t$16, %rsp\n"
        "\tmovl\t%edi, -4(%rbp)\n"
        "\tmovl\t-4(%rbp), %eax\n"
        "\tmovl\t%eax, %esi\n"
        "\tleaq	.LC0(%rip), %rdi\n"
        "\tmovl	$0, %eax\n"
        "\tcall	printf@PLT\n"
        "\tnop\n"
        "\tleave\n"
        "\tret\n"
        "\n"
        "\t.globl\tmain\n"
        "\t.type\tmain, @function\n"
        "main:\n"
        "\tpushq\t%rbp\n"
        "\tmovq	%rsp, %rbp\n";
    }
    
    std::stringstream m_ss;
    std::array<int, 4> m_reg{};
    std::array<std::string, 4> m_reg_label;
};

}

#endif  // COMPILER_2ASSEMBLY_H
