#pragma once
#include "parser.hpp"
#include "tokenizer.hpp"
#include <sstream>
#include <unordered_map>
#include <cassert>
#include <map>
#include <algorithm>
#include <vector>


class Generator{
    private:
        struct var{
            std::string name;
            size_t stack_loc;
        };
        // Removed duplicate label count line 18
        const NodeProgram m_prog;   
        std::stringstream asm_code;
        std::vector<var> m_vars {};
        std::unordered_map<std::string, std::string> m_str_vars {};
        size_t m_stack_size = 0;
        std::unordered_map<std::string, std::string> m_str_labels {};
        std::unordered_map<std::string, size_t> m_str_lens {};
        size_t m_str_count = 0;
        int m_label_count = 0;
        std::vector<size_t> m_scope {};
        bool m_inside_func = false;



        void push(const std::string& line) {
            asm_code << "    push " << line << "\n";
            m_stack_size++;
        }
        void pop(const std::string& line) {
            asm_code << "    pop " << line << "\n";
            m_stack_size--;
        }

        void begin_scope(){
            m_scope.push_back(m_vars.size());
        }

        void end_scope(){
            size_t pop_count = m_vars.size() - m_scope.back();
            asm_code << "    add rsp, " << pop_count * 8 << "\n";
            m_stack_size -= pop_count;
            for(size_t i=0;i<pop_count ; i++)
            {
                m_vars.pop_back();
            }
            m_scope.pop_back();
        }

        std::string create_label(){
            return "label" + std::to_string(m_label_count++);
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

                void operator()(const NodeTermIdent* term_ident)const{
                    const std::string var_name = term_ident->ident.value.value();
                    
                    // Logic removed that incorrectly errored on declared variables

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
                    } else {
                        auto it = std::find_if(gen->m_vars.rbegin(), gen->m_vars.rend(), [&](const var& var){
                            return var.name == var_name;
                        });
                        if(it != gen->m_vars.rend()) {
                            // Integer variable
                            gen->asm_code << "    mov rax, QWORD [rsp + " << (gen->m_stack_size - it->stack_loc - 1)*8 << "]\n";
                            gen->push("rax");
                        } else {
                            std::cerr<<"Undeclared variable: " << var_name <<std::endl;
                            exit(EXIT_FAILURE);
                        }
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

                void operator()(const NodeTermFuncCall* term_call) const{
                    // Push arguments in reverse order (Convention preference, but for stack it doesn't matter as long as we pop consistently)
                    // Let's push left-to-right (first arg pushed first). 
                    // To access them in callee: [rbp + 16 + (N-1-i)*8] ?
                    // Actually, if we push Arg1, Arg2. Stack: Arg2 (top), Arg1.
                    // Callee RBP+16 is Arg2. RBP+24 is Arg1.
                    // Let's invert push order so Arg1 is at top?
                    // Standard C (cdecl) pushes Right-to-Left. ArgN ... Arg1. Stack: Arg1 (top).
                    // let's do that.
                    for(auto it = term_call->args.rbegin(); it != term_call->args.rend(); ++it)
                    {
                        gen->gen_expr(*it);
                    }
                    gen->asm_code << "    call func_" << term_call->ident.value.value() << "\n";
                    // Clean up stack
                    size_t args_size = term_call->args.size();
                    if(args_size > 0){
                        gen->asm_code << "    add rsp, " << args_size * 8 << "\n";
                        gen->m_stack_size -= args_size;
                    }
                    gen->push("rax"); // Result
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
                    gen->asm_code << "    xor rdx, rdx\n";
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
                void operator()(const NodeBinExprEq* eq) const{
                    gen->gen_expr(eq->left);
                    gen->gen_expr(eq->right);
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    cmp rax, rbx\n";
                    gen->asm_code << "    sete al\n";
                    gen->asm_code << "    movzx rax, al\n";
                    gen->push("rax");
                }
                void operator()(const NodeBinExprNeq* neq) const{
                    gen->gen_expr(neq->left);
                    gen->gen_expr(neq->right);
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    cmp rax, rbx\n";
                    gen->asm_code << "    setne al\n";
                    gen->asm_code << "    movzx rax, al\n";
                    gen->push("rax");
                }
                void operator()(const NodeBinExprLt* lt) const{
                    gen->gen_expr(lt->left);
                    gen->gen_expr(lt->right);
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    cmp rax, rbx\n";
                    gen->asm_code << "    setl al\n";
                    gen->asm_code << "    movzx rax, al\n";
                    gen->push("rax");
                }
                void operator()(const NodeBinExprGt* gt) const{
                    gen->gen_expr(gt->left);
                    gen->gen_expr(gt->right);
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    cmp rax, rbx\n";
                    gen->asm_code << "    setg al\n";
                    gen->asm_code << "    movzx rax, al\n";
                    gen->push("rax");
                }
                void operator()(const NodeBinExprLte* lte) const{
                    gen->gen_expr(lte->left);
                    gen->gen_expr(lte->right);
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    cmp rax, rbx\n";
                    gen->asm_code << "    setle al\n";
                    gen->asm_code << "    movzx rax, al\n";
                    gen->push("rax");
                }
                void operator()(const NodeBinExprGte* gte) const{
                    gen->gen_expr(gte->left);
                    gen->gen_expr(gte->right);
                    gen->pop("rbx");
                    gen->pop("rax");
                    gen->asm_code << "    cmp rax, rbx\n";
                    gen->asm_code << "    setge al\n";
                    gen->asm_code << "    movzx rax, al\n";
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

        void gen_scope(const NodeScope* scope)
        {
            begin_scope();
            for(const auto* stmt : scope->stmts)
            {
                gen_stmt(*stmt);
            }
            end_scope();
        }

        void gen_stmt(const NodeStmt& stmt) {

            struct StmtVisitor{
                Generator* gen;
                
                void operator()(NodeStmtExit* stmt_exit) const
                {
                    gen->gen_expr(stmt_exit->expr);
                    if(gen->m_inside_func)
                    {
                        // Return from function
                        gen->pop("rax");
                        gen->asm_code << "    leave\n";
                        gen->asm_code << "    ret\n";
                    }
                    else
                    {
                        // Exit program
                        gen->asm_code << "    mov rax, 60\n";
                        gen->pop("rdi");
                        gen->asm_code << "    syscall\n";
                    }
                }
                void operator()(NodeStmtHope* stmt_hope) const
                {
                    auto scope_start = gen->m_scope.empty() ? gen->m_vars.begin() : gen->m_vars.begin() + gen->m_scope.back();
                    auto it = std::find_if(scope_start, gen->m_vars.end(), [&](const var& var){
                        return var.name == stmt_hope->ident.value.value();
                    });
                    if(it != gen->m_vars.end())
                    {
                        std::cerr<<"Variable already declared in this scope: " << stmt_hope->ident.value.value() <<std::endl;
                        exit(EXIT_FAILURE);
                    }

                    gen->m_vars.push_back({.name=stmt_hope->ident.value.value(), .stack_loc=gen->m_stack_size});
                    gen->gen_expr(stmt_hope->expr);
                }

                void operator()(const NodeScope* scope) const
                {
                    gen->gen_scope(scope);
                }

                void operator()(NodeStmtMaybe* stmt_maybe) const {
                    std::string label_end = gen->create_label();
                    std::string label_next = gen->create_label();

                    // IF
                    gen->gen_expr(stmt_maybe->condition);
                    gen->pop("rax");
                    gen->asm_code << "    test rax, rax \n";
                    gen->asm_code << "    jz " << label_next << "\n";
                    gen->gen_scope(stmt_maybe->scope);
                    gen->asm_code << "    jmp " << label_end << "\n";
                    gen->asm_code << label_next << ":\n";

                    // ELIFS
                    for(const auto* elif : stmt_maybe->elifs)
                    {
                        label_next = gen->create_label();
                        gen->gen_expr(elif->condition);
                        gen->pop("rax");
                        gen->asm_code << "    test rax, rax \n";
                        gen->asm_code << "    jz " << label_next << "\n";
                        gen->gen_scope(elif->scope);
                        gen->asm_code << "    jmp " << label_end << "\n";
                        gen->asm_code << label_next << ":\n";
                    }

                    // ELSE
                    if(stmt_maybe->else_stmt.has_value())
                    {
                        gen->gen_scope(stmt_maybe->else_stmt.value()->scope);
                    }

                    gen->asm_code << label_end << ":\n";
                }

                void operator()(NodeStmtWait* stmt) const {
                    std::string label_start = gen->create_label();
                    std::string label_end = gen->create_label();
                    gen->asm_code << label_start << ":\n";
                    gen->gen_expr(stmt->condition);
                    gen->pop("rax");
                    gen->asm_code << "    test rax, rax \n";
                    gen->asm_code << "    jz " << label_end << "\n";
                    gen->gen_scope(stmt->scope);
                    gen->asm_code << "    jmp " << label_start << "\n";
                    gen->asm_code << label_end << ":\n";
                }

                void operator()(NodeStmtMoveOn* stmt) const {
                    // This should not be visited directly anymore via proper parsing, 
                    // but keeping basic impl just in case or empty
                    gen->gen_scope(stmt->scope);
                }
               
                 void operator()(NodeStmtOrMaybe* stmt) const {
                    // Should not be visited directly
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
                void operator()(NodeStmtAssign* stmt_assign) const
                {
                    auto it = std::find_if(gen->m_vars.rbegin(), gen->m_vars.rend(), [&](const var& var){
                        return var.name == stmt_assign->ident.value.value();
                    });

                    if(it != gen->m_vars.rend())
                    {
                        // Found in current scope - Update it
                        gen->gen_expr(stmt_assign->expr);
                        gen->pop("rax");
                        gen->asm_code << "    mov [rsp + " << (gen->m_stack_size - it->stack_loc - 1)*8 << "], rax\n";
                    }
                    else
                    {
                         // Not found in current scope - Implicitly declare it (Shadowing)
                        gen->m_vars.push_back({.name=stmt_assign->ident.value.value(), .stack_loc=gen->m_stack_size});
                        gen->gen_expr(stmt_assign->expr);
                    }
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

                void operator()(NodeStmtThen* stmt_then) const
                {
                    gen->asm_code << "    mov rax, 1\n";
                    gen->asm_code << "    mov rdi, 1\n";
                    gen->asm_code << "    lea rsi, [rel newline_const]\n";
                    gen->asm_code << "    mov rdx, 1\n";
                    gen->asm_code << "    syscall\n";
                }

                void operator()(NodeFuncDef* func_def) const
                {
                    // Function definition should not be executed inline in main flow. 
                    // It is handled by the first pass in gen_program or skipped if we iterate naively.
                    // We will generate the code here, but we assume gen_program calls this at the right time (outside _start).
                    
                    std::string func_label = "func_" + func_def->name.value.value();
                    gen->asm_code << "\n" << func_label << ":\n";
                    gen->asm_code << "    push rbp\n";
                    gen->asm_code << "    mov rbp, rsp\n";
                    
                    // Reset stack tracking for function scope
                    size_t old_stack_size = gen->m_stack_size;
                    std::vector<var> old_vars = gen->m_vars;
                    gen->m_stack_size = 0;
                    gen->m_vars.clear();
                    gen->m_inside_func = true;
                    
                    // Bind Arguments
                    // Stack at entry: [RetIP] [OldRBP] [Arg1] [Arg2] ... (Assuming Right-to-Left push)
                    // Arg1 is at RBP+16. Arg2 is at RBP+24.
                    // We want to access them as if they are local variables.
                    // We can Copy them to local stack or map them.
                    // Mapping is complex because 'gen_term' expects 'rsp + offset'. 
                    // Simplest: Push them from [rbp+offset] to stack to make them local vars.
                    
                    size_t arg_count = func_def->args.size();
                    for(size_t i=0; i<arg_count; ++i)
                    {
                        // Arg i (0-index) is at RBP + 16 + i*8
                        gen->asm_code << "    push QWORD [rbp + " << 16 + i*8 << "]\n";
                        gen->m_stack_size++;
                        gen->m_vars.push_back({.name=func_def->args[i].second.value.value(), .stack_loc=gen->m_stack_size-1}); // stack_loc matches current pos
                    }
                    
                    gen->gen_scope(func_def->scope);
                    
                    // Default return 0 if no generic return found (just safety)
                    gen->asm_code << "    mov rax, 0\n";
                    gen->asm_code << "    leave\n";
                    gen->asm_code << "    ret\n";
                    
                    // Restore state
                    gen->m_inside_func = false;
                    gen->m_vars = old_vars;
                    gen->m_stack_size = old_stack_size;
                }

            };
            StmtVisitor visitor{.gen=this};
            std::visit(visitor, stmt.var);

        }

        [[nodiscard]] std::string gen_program() {
            asm_code << "section .text\n";
            asm_code << "global _start\n";
            // Generate Functions First
            for(const NodeStmt & stmt: m_prog.stmts)
            {
                if(std::holds_alternative<NodeFuncDef*>(stmt.var)) {
                    gen_stmt(stmt);
                }
            }
            
            asm_code << "\n_start:\n";
            // Generate Main Body (Skip functions)
            for(const NodeStmt & stmt: m_prog.stmts)
            {
                 if(!std::holds_alternative<NodeFuncDef*>(stmt.var)) {
                    gen_stmt(stmt);
                }
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
            asm_code << "\nsection .data\n";
            asm_code << "newline_const: db 10, 0\n";

            if(!m_str_labels.empty()){
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