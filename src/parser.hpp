#pragma once
#include "arena.hpp"
#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <cstdlib>
#include <variant>
#include "types.hpp"
using ::bin_prec;

// Forward declarations
struct NodeExpr;

struct NodeTermIntLit{
    Token int_lit;
};

struct NodeTermIdent{
    Token ident;
};

struct NodeTerm{
    std::variant<NodeTermIntLit*, NodeTermIdent*> var;
};

struct NodeBinExprAdd {
    NodeExpr* left;
    NodeExpr* right;
};

struct NodeBinExprSub {
    NodeExpr* left;
    NodeExpr* right;
};
struct NodeBinExprDiv {
    NodeExpr* left;
    NodeExpr* right;
};
struct NodeBinExprMulti{
     NodeExpr* left;
     NodeExpr* right;
};

struct NodeBinExpr{
    std::variant<NodeBinExprAdd*, NodeBinExprMulti*> var;
};

struct NodeExpr{
    std::variant<NodeTerm*, NodeBinExpr*> var;
};

struct NodeStmtExit{
    NodeExpr* expr;
};

struct NodeStmtHope{
    Token ident;
    NodeExpr* expr;
};

struct NodeStmt{
    std::variant<NodeStmtExit*, NodeStmtHope*> var;
};

struct NodeProgram{
    std::vector<NodeStmt> stmts;
};


class Parser {
    public:
        inline explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)), m_alloc(1024 * 1024*4)
        {}


        std::optional<NodeTerm*> parse_term()
        {
            if(auto int_lit =try_consume(TokenType::int_lit))
            {
                auto Term_expr_int_lit = m_alloc.alloc<NodeTermIntLit>();
                Term_expr_int_lit->int_lit=int_lit.value();
                auto expr= m_alloc.alloc<NodeTerm>();
                expr->var=Term_expr_int_lit;
                return expr;
            }
            else if(auto ident = try_consume(TokenType::ident))
            {
                auto Term_expr_ident = m_alloc.alloc<NodeTermIdent>();
                Term_expr_ident->ident = ident.value();
                auto expr = m_alloc.alloc<NodeTerm>();
                expr->var = Term_expr_ident;
                return expr;
            }
            return {};
        }

        std::optional<NodeExpr*> parse_expr(int min_prec=0)
        { 
            
            std::optional<NodeTerm*> term_lhs=parse_term();
            if(!term_lhs.has_value())
            {
                return {};
            }

            auto expr_lhs=m_alloc.alloc<NodeExpr>();
            expr_lhs->var=term_lhs.value();
            
            while(true)
            {
                std::optional<Token> curr_tok = peek();
                std::optional<int> prec;

                if(curr_tok.has_value())
                {
                    prec = bin_prec(curr_tok->type);
                    if(!prec.has_value() || prec.value() < min_prec)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
                Token op = consume();
                int next_min_prec = prec.value()+1;
                auto expr_rhs = parse_expr(next_min_prec);
                if(!expr_rhs.has_value())
                {
                    std::cerr<<"Invalid right-hand side expression"<<std::endl;
                    exit(EXIT_FAILURE);
                }

                auto expr = m_alloc.alloc<NodeExpr>();
                auto expr_lhs2=m_alloc.alloc<NodeExpr>();
                if(op.type == TokenType::plus)
                {
                    auto add = m_alloc.alloc<NodeBinExprAdd>();
                    expr_lhs2->var = expr_lhs->var;
                    add->left=expr_lhs2;
                    add->right=expr_rhs.value();
                    auto bin_expr = m_alloc.alloc<NodeBinExpr>();
                    bin_expr->var = add;
                    expr->var = bin_expr;
                }
                else if(op.type == TokenType::mul)
                {
                    auto multi = m_alloc.alloc<NodeBinExprMulti>();
                    expr_lhs2->var = expr_lhs->var;
                    multi->left=expr_lhs2;
                    multi->right=expr_rhs.value();
                    auto bin_expr = m_alloc.alloc<NodeBinExpr>();
                    bin_expr->var = multi;
                    expr->var = bin_expr;
                }
                expr_lhs->var = expr->var;
            }

            return expr_lhs;
        }



        std::optional<NodeStmt> parse_stmt()
        {
            if(peek().has_value() && peek().value().type==TokenType::bye && peek(1).has_value() && peek(1).value().type==TokenType::open_paren)
            {
                consume();
                consume();
                auto stmt_exit = m_alloc.alloc<NodeStmtExit>();
                if(auto node_expr=parse_expr()) //parsing expression after 'bye' token
                {
                    stmt_exit->expr=node_expr.value();
                }
                else{
                    std::cerr<<"Unexpected token encountered during parsing"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                try_consume(TokenType::close_paren, "expecting close parenthesis ')'");
                try_consume(TokenType::semi, "expecting semicolon ';'");
                return NodeStmt{.var=stmt_exit};
            }
            else if(peek().has_value() && peek().value().type == TokenType::hope && peek(1).has_value() && peek(1).value().type==TokenType::ident && peek(2).has_value() && peek(2).value().type==TokenType::eq)
            {
                consume(); // consume 'hope' keyword
                auto stmt_hope = m_alloc.alloc<NodeStmtHope>();
                stmt_hope->ident=consume(); // consume identifier
                consume(); // consume '='
                if(auto expr=parse_expr()) //parsing expression after 'hope' token
                {
                    stmt_hope->expr=expr.value();
                }
                else{
                    std::cerr<<"Invalid Expression after 'hope'"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                try_consume(TokenType::semi, "expecting semicolon ';'"); 
                return NodeStmt{.var=stmt_hope};
            }
            
            return {};
        }


        std::optional<NodeProgram> parse_prog()
        {
            NodeProgram prog;
            while(peek().has_value())
            {
                if(auto stmt=parse_stmt())
                {
                    prog.stmts.push_back(stmt.value());
                }
                else{
                    std::cerr<<"Invalid Statement encountered during parsing"<<std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            return prog;
        }



    private:
    [[nodiscard]] inline std::optional<Token> peek(int offset=0) const //nodiscard : to indicate the return value should not be ignored
    {
        if(position+offset >= m_tokens.size())
        {
            return {};
        }
        else
        {
            return m_tokens[position+offset];
        }
    }

    inline Token consume() //function to consume the current token and advance the position
    {
        return m_tokens[position++];
    }

    inline Token try_consume(TokenType type,const std::string& err_msg) //function to consume the current token if it matches the expected type
    {
        if(peek().has_value() && peek().value().type==type)
        {
            return consume();
        }
        else
        {
            std::cerr<<err_msg<<std::endl;
            exit(EXIT_FAILURE);
        }
    }


    inline std::optional<Token> try_consume(TokenType type) //function to consume the current token if it matches the expected type
    {
        if(peek().has_value() && peek().value().type==type)
        {
            return consume();
        }
        else
        {
            return {};
        }
    }



    ArenaAllocation m_alloc; //m_alloc=arena allocator instance

        const std::vector<Token> m_tokens; //m_tokens=store the list of tokens
        size_t position = 0;
};