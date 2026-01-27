#pragma once
#include "parser.hpp"
#include "tokenizer.hpp"
#include <sstream>
#include <unordered_map>
#include <cassert>

class Generator{
    private:
        struct var{
            size_t stack_loc;
        };

        const NodeProgram m_prog;   
        std::stringstream asm_code;
        std::unordered_map<std::string, var> m_vars {};
        std::unordered_map<std::string, std::string> m_str_vars {}; // string variable -> label mapping
        size_t m_stack_size = 0;
        std::unordered_map<std::string, std::string> m_str_labels {};
        std::unordered_map<std::string, size_t> m_str_lens {};
        size_t m_str_count = 0;
        void push(const std::string& line) {
            asm_code << "    push " << line << "\n";
            m_stack_size++;
        }
        void pop(const std::string& line) {
            asm_code << "    pop " << line << "\n";
            m_stack_size--;
        }



    public:
        inline explicit Generator(const NodeProgram& prog) : m_prog(std::move(prog)) {
        }   

        void gen_term(const NodeTerm* term)
        {
            struct TermVisitor{
                Generator*gen;
                void operator()(const NodeTermIntLit* int_lit){
                    
                    gen->asm_code << "    mov rax, " << int_lit->int_lit.value.value() << "\n";
                    gen->push("rax");
                }

                void operator()(const NodeTermIdent* ident){
                    const std::string var_name = ident->ident.value.value();
                    
                    // Check if it's a string variable
                    if(gen->m_str_vars.find(var_name) != gen->m_str_vars.end()){
                        const std::string label = gen->m_str_vars.at(var_name);
                        // Find the original string to get its length
                        size_t str_len = 0;
                        for(const auto& p : gen->m_str_labels){
                            if(p.second == label){
                                str_len = gen->m_str_lens.at(p.first);
                                break;
                            }
                        }
                        gen->asm_code << "    mov rax, 1\n";
                        gen->asm_code << "    mov rdi, 1\n";
                        gen->asm_code << "    lea rsi, [rel " << label << "]\n";
                        gen->asm_code << "    mov rdx, " << str_len << "\n";
                        gen->asm_code << "    syscall\n";
                    } else if(gen->m_vars.find(var_name) != gen->m_vars.end()) {
                        // Integer variable
                        const auto& var = gen->m_vars.at(var_name);
                        gen->asm_code << "    mov rax, QWORD [rsp + " << (gen->m_stack_size - var.stack_loc - 1)*8 << "]\n";
                        gen->push("rax");
                    } else {
                        std::cerr<<"Undeclared variable: " << var_name <<std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                void operator()(const NodeTermParen* term_paren){
                    gen->gen_expr(term_paren->expr);
                }
                void operator()(const NodeTermStringLit* string_lit){
                    const std::string s = string_lit->string_lit.value.value();
                    std::string label;
                    if(gen->m_str_labels.find(s) == gen->m_str_labels.end()){
                        label = "msg_" + std::to_string(gen->m_str_count++);
                        gen->m_str_labels.emplace(s, label);
                        gen->m_str_lens.emplace(s, s.size());
                    } else {
                        label = gen->m_str_labels.at(s);
                    }
                    gen->asm_code << "    mov rax, 1\n";
                    gen->asm_code << "    mov rdi, 1\n";
                    gen->asm_code << "    lea rsi, [rel " << label << "]" << "\n";
                    gen->asm_code << "    mov rdx, " << gen->m_str_lens.at(s) << "\n";
                    gen->asm_code << "    syscall\n";
                }

            };

            TermVisitor visitor({.gen=this});
            std::visit(visitor, term->var);
            
        }

        void gen_bin_expr(const NodeBinExpr* bin_expr)
        {
            struct BinExprVisitor{
                Generator* gen;

                void operator()(const NodeBinExprSub* sub) const{
                    gen->gen_expr(sub->left);
                    gen->gen_expr(sub->right);
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    sub rax, rbx\n";
                    gen->push("rax");
                }
                void operator()(const NodeBinExprDiv* div) const{
                    gen->gen_expr(div->left);
                    gen->gen_expr(div->right);  
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    div rbx\n";
                    gen->push("rax");
                }

                void operator()(const NodeBinExprAdd* add) const{
                    gen->gen_expr(add->left);
                    gen->gen_expr(add->right);
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    add rax, rbx\n";
                    gen->push("rax");
                }
                void operator()(const NodeBinExprMulti* multi) const{
                    gen->gen_expr(multi->left);
                    gen->gen_expr(multi->right);
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    mul rbx\n";
                    gen->push("rax");
                }

            };

            BinExprVisitor visitor{.gen=this};
            std::visit(visitor, bin_expr->var);

        }




        void gen_expr(const NodeExpr* expr) {
            
            struct ExprVisitor{
                Generator* gen;

                void operator()(NodeTerm* term)const{
                    gen->gen_term(term);
                }
                void operator()(NodeBinExpr* bin_exp) const{
                    gen->gen_bin_expr(bin_exp);
                }

            };
            ExprVisitor visitor{.gen=this};
            std::visit(visitor, expr->var);

        }
        void gen_stmt(const NodeStmt& stmt) {

            struct StmtVisitor{
                Generator* gen;
                
                void operator()(NodeStmtExit* stmt_exit) const
                {
                    gen->gen_expr(stmt_exit->expr);
                    gen->asm_code << "    mov rax, 60\n";
                    gen->pop("rdi");
                    gen->asm_code << "    syscall\n";
                }
                void operator()(NodeStmtHope* stmt_hope) const
                {
                    if(gen->m_vars.find(stmt_hope->ident.value.value()) != gen->m_vars.end())
                    {
                        std::cerr<<"Variable already declared: " << stmt_hope->ident.value.value() <<std::endl;
                        exit(EXIT_FAILURE);
                    }

                    gen->m_vars.insert(std::make_pair(stmt_hope->ident.value.value(), var{.stack_loc=gen->m_stack_size}));
                    gen->gen_expr(stmt_hope->expr);
                }
                void operator()(NodeStmtDillusion* stmt_dillusion) const
                {
                    const std::string var_name = stmt_dillusion->ident.value.value();
                    if(gen->m_str_vars.find(var_name) != gen->m_str_vars.end())
                    {
                        std::cerr<<"String variable already declared: " << var_name <<std::endl;
                        exit(EXIT_FAILURE);
                    }

                    // Extract the string from the expression
                    if(std::holds_alternative<NodeTerm*>(stmt_dillusion->expr->var)){
                        NodeTerm* term = std::get<NodeTerm*>(stmt_dillusion->expr->var);
                        if(std::holds_alternative<NodeTermStringLit*>(term->var)){
                            NodeTermStringLit* str_lit = std::get<NodeTermStringLit*>(term->var);
                            const std::string s = str_lit->string_lit.value.value();
                            std::string label;
                            if(gen->m_str_labels.find(s) == gen->m_str_labels.end()){
                                label = "msg_" + std::to_string(gen->m_str_count++);
                                gen->m_str_labels.emplace(s, label);
                                gen->m_str_lens.emplace(s, s.size());
                            } else {
                                label = gen->m_str_labels.at(s);
                            }
                            gen->m_str_vars.emplace(var_name, label);
                            return;
                        }
                    }
                    std::cerr<<"String variable must be initialized with a string literal"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                void operator()(NodeStmtTellMe* stmt_tell_me) const
                {
                    bool is_string = false;
                    
                    // Check if it's a string literal
                    if(std::holds_alternative<NodeTerm*>(stmt_tell_me->expr->var)){
                        NodeTerm* term = std::get<NodeTerm*>(stmt_tell_me->expr->var);
                        if(std::holds_alternative<NodeTermStringLit*>(term->var)){
                            is_string = true;
                        }
                        // Check if it's a string variable (identifier)
                        else if(std::holds_alternative<NodeTermIdent*>(term->var)){
                            NodeTermIdent* ident = std::get<NodeTermIdent*>(term->var);
                            const std::string var_name = ident->ident.value.value();
                            if(gen->m_str_vars.find(var_name) != gen->m_str_vars.end()){
                                is_string = true;
                            }
                        }
                    }
                    
                    gen->gen_expr(stmt_tell_me->expr);
                    if(!is_string){
                        gen->pop("rdi");
                        gen->asm_code << "    call print_int\n";
                    }
                }

            };
            StmtVisitor visitor{.gen=this};
            std::visit(visitor, stmt.var);

        }

        [[nodiscard]] std::string gen_program() {
            asm_code << "section .text\n";
            asm_code << "global _start\n";
            asm_code << "_start:\n";
            
            // First pass: count variables to allocate stack space
            int var_count = 0;
            for(const NodeStmt & stmt: m_prog.stmts)
            {
                if(std::holds_alternative<NodeStmtHope*>(stmt.var)) {
                    var_count++;
                }
            }
            
            // Allocate stack space

            
            for(const NodeStmt & stmt: m_prog.stmts)
            {
                gen_stmt(stmt);
            }
            
            
            asm_code << "    mov rax, 60\n"; // syscall: exit
            asm_code << "    mov rdi, 0\n";
            asm_code << "    syscall\n";
            
            // Helper function to print integer
            asm_code << "\nprint_int:\n";
            asm_code << "    push rbp\n";
            asm_code << "    mov rbp, rsp\n";
            asm_code << "    sub rsp, 32\n";
            asm_code << "    mov rax, rdi\n";
            asm_code << "    lea rsi, [rel buffer]\n";
            asm_code << "    mov rcx, 0\n";
            asm_code << "    cmp rax, 0\n";
            asm_code << "    jge .L1\n";
            asm_code << "    neg rax\n";
            asm_code << "    mov byte [rsi], '-'\n";
            asm_code << "    inc rsi\n";
            asm_code << "    inc rcx\n";
            asm_code << ".L1:\n";
            asm_code << "    mov r8, 10\n";
            asm_code << "    mov r9, 0\n";
            asm_code << ".L2:\n";
            asm_code << "    xor edx, edx\n";
            asm_code << "    div r8\n";
            asm_code << "    add dl, '0'\n";
            asm_code << "    push rdx\n";
            asm_code << "    inc r9\n";
            asm_code << "    cmp rax, 0\n";
            asm_code << "    jne .L2\n";
            asm_code << ".L3:\n";
            asm_code << "    pop rax\n";
            asm_code << "    mov byte [rsi], al\n";
            asm_code << "    inc rsi\n";
            asm_code << "    inc rcx\n";
            asm_code << "    dec r9\n";
            asm_code << "    jne .L3\n";
            asm_code << "    mov byte [rsi], 10\n";
            asm_code << "    inc rcx\n";
            asm_code << "    mov rax, 1\n";
            asm_code << "    mov rdi, 1\n";
            asm_code << "    lea rsi, [rel buffer]\n";
            asm_code << "    mov rdx, rcx\n";
            asm_code << "    syscall\n";
            asm_code << "    leave\n";
            asm_code << "    ret\n";
            asm_code << "\nsection .bss\n";
            asm_code << "buffer: resb 32\n";

            // Emit data section for string literals
            if(!m_str_labels.empty()){
                asm_code << "\nsection .data\n";
                for(const auto &p : m_str_labels){
                    const std::string &str = p.first;
                    const std::string &label = p.second;
                    // escape double quotes by replacing with \" if present
                    std::string out;
                    for(char c: str){
                        if(c == '"') out += "\\\"";
                        else out += c;
                    }
                    asm_code << label << ": db \"" << out << "\"" << ", 0\n";
                }
            }
            
            return asm_code.str();
        }
        
        
        
};