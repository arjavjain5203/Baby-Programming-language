#pragma once

#include <string>
#include <optional>
#include <vector>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include "types.hpp"

class Tokenizer {
public:
    inline explicit Tokenizer(std::string src) : source(std::move(src)) //constructor to initialize source string
    {
    }

    inline std::vector<Token> tokenize()
    {
        std::string buf; //buffer to hold characters for multi-character tokens
        std::vector<Token> tokens;

        while(peek().has_value())
        {
            if(std::isalpha(peek().value()))
            {
                buf.push_back(consume());
                while(peek().has_value() && std::isalnum(peek().value()))
                {
                    buf.push_back(consume());
                }
                if(buf=="bye")
                {
                    tokens.push_back({.type=TokenType::bye});
                    buf.clear();
                    continue;
                }
                else if(buf=="hope")
                {
                    tokens.push_back({.type=TokenType::hope});
                    buf.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type=TokenType::ident,.value=buf});
                    buf.clear();
                    continue;
                }
            }

            else if(std::isdigit(peek().value()))
            {
                buf.push_back(consume());
                while(peek().has_value() && std::isdigit(peek().value()))
                {
                    buf.push_back(consume());
                }
                tokens.push_back({.type=TokenType::int_lit,.value=buf});
                buf.clear();
                continue;
            }
            else if(peek().value()=='(')
            {
                consume();
                tokens.push_back({.type=TokenType::open_paren});
                continue;
            }
            else if(peek().value()==')')
            {
                consume();
                tokens.push_back({.type=TokenType::close_paren});
                continue;
            }
            else if(peek().value()==';')
            {
                consume();
                tokens.push_back({.type=TokenType::semi});
                continue;
            }
            else if(peek().value()=='=')
            {
                consume();
                tokens.push_back({.type=TokenType::eq});
                continue;
            }
            else if(std::isspace(peek().value()))
            {           
                consume();
                continue;
            }
            else
            {
                std::cerr<<"you sucks! Unexpected character: '" << peek().value() << "' (ASCII: " << (int)peek().value() << ")"<<std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return tokens;
    }
private:

    [[nodiscard]] inline std::optional<char> peek(int offset=0) const //nodiscard : to indicate the return value should not be ignored
    {
        if(position+offset >= source.size())
        {
            return {};
        }
        else
        {
            return source[position+offset];
        }
    }

    inline char consume() //function to consume the current character and advance the position
    {
        return source[position++];
    }


    const std::string source;
    size_t position = 0;

};