#pragma once

#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <cstdlib>
#include <variant>
#include "types.hpp"

struct NodeExprIntLit{
        Token int_lit;
    };

struct NodeExprIdent{
        Token ident;
    };

struct NodeExpr{
        std::variant<NodeExprIntLit, NodeExprIdent> var;
    };  

struct NodeStmtExit{
        NodeExpr expr;
    };

struct NodeStmtHope{
        Token ident;
        NodeExpr expr;
    };

struct NodeStmt{
        std::variant<NodeStmtExit,NodeStmtHope> var;
    };

struct NodeProgram{
        std::vector<NodeStmt> stmts;
    };


class Parser {
    public:
        inline explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {

        }


        std::optional<NodeExpr> parse_expr()
        { 
            if(peek().has_value() && peek().value().type==TokenType::int_lit)
            {
                return NodeExpr{.var=NodeExprIntLit{.int_lit=consume()}};
            }
            else if(peek().has_value() && peek().value().type==TokenType::ident)
            {
                return NodeExpr{.var=NodeExprIdent{.ident=consume()}};
            }
            else{
                return {};
            }
        }

        std::optional<NodeStmt> parse_stmt()
        {
            if(peek().has_value() && peek().value().type==TokenType::bye && peek(1).has_value() && peek(1).value().type==TokenType::open_paren)
            {
                consume();
                consume();
                NodeStmtExit stmt_exit;
                if(auto node_expr=parse_expr()) //parsing expression after 'bye' token
                {
                    stmt_exit=NodeStmtExit{.expr=node_expr.value()};
                }
                else{
                    std::cerr<<"Unexpected token encountered during parsing"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                if(peek().has_value() && peek().value().type==TokenType::close_paren)
                {
                    consume();
                }
                else{
                    std::cerr<<"expecting close parenthesis ')'"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                if(peek().has_value() && peek().value().type==TokenType::semi)
                {
                    consume();
                }
                else{
                    std::cerr<<"expecting semicolon ';'"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                return NodeStmt{.var=stmt_exit};
            }
            else if(peek().has_value() && peek().value().type == TokenType::hope && peek(1).has_value() && peek(1).value().type==TokenType::ident && peek(2).has_value() && peek(2).value().type==TokenType::eq)
            {
                consume(); // consume 'hope' keyword
                auto stmt_hope=NodeStmtHope{.ident=consume()}; // consume identifier
                consume(); // consume '='
                if(auto expr=parse_expr()) //parsing expression after 'hope' token
                {
                    stmt_hope.expr=expr.value();
                }
                else{
                    std::cerr<<"Invalid Expression after 'hope'"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                if(peek().has_value() && peek().value().type ==TokenType::semi)
                {
                    consume();
                }
                else{
                    std::cerr<<"expecting semicolon ';'"<<std::endl;
                    exit(EXIT_FAILURE);
                }
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

        const std::vector<Token> m_tokens; //m_tokens=store the list of tokens
        size_t position = 0;
};