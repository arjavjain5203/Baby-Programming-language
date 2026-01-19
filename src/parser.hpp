#pragma once

#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <cstdlib>
#include "types.hpp"

struct Nodeexpr{
        Token int_lit;
    };  

struct Nodeexit{
        Nodeexpr expr;
    };



class Parser {
    public:
        inline explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {

        }


        std::optional<Nodeexpr> parse_expr()
        { 
            if(peak().has_value() && peak().value().type==TokenType::int_lit)
            {
                return Nodeexpr{.int_lit=consume()};
            }else{
                return {};
            }
        }


        std::optional<Nodeexit> parse()
        {
            std::optional<Nodeexit> exit_node;
            while(peak().has_value())
            {
                if(peak().value().type==TokenType::bye)
                {
                    consume();
                    if(auto node_expr=parse_expr()) //parsing expression after 'bye' token
                    {
                        exit_node=Nodeexit{.expr=node_expr.value()};
                        if(peak().has_value() && peak().value().type ==TokenType::semi)
                        {
                            consume();
                        }
                        else{
                            std::cerr<<"Unexpected token encountered during parsing"<<std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;
                    }
                    else{
                        std::cerr<<"Unexpected token encountered during parsing"<<std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                else {
                    std::cerr<<"Unexpected token encountered during parsing"<<std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            position=0;
            return exit_node;

        }



    private:
            [[nodiscard]] inline std::optional<Token> peak(int ahead=1) const //nodiscard : to indicate the return value should not be ignored
    {
        if(position+ahead > m_tokens.size())
        {
            return {};
        }
        else
        {
            return m_tokens[position];
        }
    }

    inline Token consume() //function to consume the current token and advance the position
    {
        return m_tokens[position++];
    }

        const std::vector<Token> m_tokens; //m_tokens=store the list of tokens
        size_t position = 0;
};