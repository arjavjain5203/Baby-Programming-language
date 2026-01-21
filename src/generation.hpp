#pragma once
#include "parser.hpp"
#include "tokenizer.hpp"
#include <sstream>
#include <unordered_map>


class Generator{
    private:
        struct var{
            size_t stack_loc;
        };

        const NodeProgram m_prog;   
        std::stringstream asm_code;
        std::unordered_map<std::string, var> m_vars {};
        size_t m_stack_size = 0;
        void push(const std::string& line) {
            asm_code << "    push " << line << "\n";
            m_stack_size++;
        }
        void pop(const std::string& line) {
            asm_code << "    pop " << line << "\n";
            m_stack_size--;
        }

    public:
        inline explicit Generator(NodeProgram prog) : m_prog(std::move(prog)) {
        }   

        void gen_expr(const NodeExpr& expr) {
            
            struct ExprVisitor{
                Generator* gen;

                void operator()(const NodeExprIntLit& expr_int_lit){
                    gen->asm_code << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                    gen->push("rax");
                }

                void operator()(const NodeExprIdent& expr_ident){
                    if(gen->m_vars.find(expr_ident.ident.value.value()) == gen->m_vars.end())
                    {
                        std::cerr<<"Undeclared variable: " << expr_ident.ident.value.value() <<std::endl;
                        exit(EXIT_FAILURE);
                    }
                    const auto& var = gen->m_vars.at(expr_ident.ident.value.value());
                    std::stringstream offset;
                    offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_loc - 1)*8 << "]\n";
                    gen->push(offset.str());

                }

            };
            ExprVisitor visitor{.gen=this};
            std::visit(visitor, expr.var);

        }



        void gen_stmt(const NodeStmt& stmt) {

            struct StmtVisitor{
                Generator* gen;
                
                void operator()(const NodeStmtExit& stmt_exit) const
                {
                    gen->gen_expr(stmt_exit.expr);
                    gen->asm_code << "    mov rax, 60\n";
                    gen->pop("rdi");
                    gen->asm_code << "    syscall\n";
                }
                void operator()(const NodeStmtHope& stmt_hope)
                {
                    if(gen->m_vars.find(stmt_hope.ident.value.value()) != gen->m_vars.end())
                    {
                        std::cerr<<"Variable already declared: " << stmt_hope.ident.value.value() <<std::endl;
                        exit(EXIT_FAILURE);
                    }

                    gen->m_vars.insert(std::make_pair(stmt_hope.ident.value.value(), var{.stack_loc=gen->m_stack_size}));
                    gen->gen_expr(stmt_hope.expr);


                }

            };
            StmtVisitor visitor{.gen=this};
            std::visit(visitor, stmt.var);

        }

        [[nodiscard]] std::string gen_program() {
            asm_code << "global _start\n";
            asm_code << "_start:\n";
            
            // First pass: count variables to allocate stack space
            int var_count = 0;
            for(const NodeStmt & stmt: m_prog.stmts)
            {
                if(std::holds_alternative<NodeStmtHope>(stmt.var)) {
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
            return asm_code.str();
        }
        
        
        
};