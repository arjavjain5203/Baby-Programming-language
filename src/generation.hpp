#pragma once
#include "parser.hpp"
#include "tokenizer.hpp"
#include <sstream>


class Generator{
    public:
        inline explicit Generator(Nodeexit root) : m_root(std::move(root)) {

        }   
        [[nodiscard]] std::string generate() const{
                std::stringstream asm_code;
                asm_code << "global _start\n";
                asm_code << "_start:\n";
                asm_code << "    mov rax, 60\n"; // syscall: exit
                asm_code << "    mov rdi, " << std::stoi(m_root.expr.int_lit.value.value()) << "\n"; // exit code
                asm_code << "    syscall\n";
                return asm_code.str();
        }


    
    private:
        const Nodeexit m_root;    
};