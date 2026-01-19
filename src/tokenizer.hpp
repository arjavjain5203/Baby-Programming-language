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

        while(peak().has_value())
        {
            if(std::isalpha(peak().value()))
            {
                buf.push_back(consume());
                while(peak().has_value() && std::isalnum(peak().value()))
                {
                    buf.push_back(consume());
                }
                if(buf=="bye")
                {
                    tokens.push_back({.type=TokenType::bye});
                    buf.clear();
                    continue;
                }
                else
                {
                    std::cerr<<"you sucks! "<<std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if(std::isdigit(peak().value()))
            {
                buf.push_back(consume());
                while(peak().has_value() && std::isdigit(peak().value()))
                {
                    buf.push_back(consume());
                }
                tokens.push_back({.type=TokenType::int_lit,.value=buf});
                buf.clear();
                continue;
            }
            else if(peak().value()==';')
            {
                consume();
                tokens.push_back({.type=TokenType::semi});
                continue;
            }
            else if(std::isspace(peak().value()))
            {           
                consume();
                continue;
            }
            else
            {
                std::cerr<<"you sucks! "<<std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return tokens;
    }
private:

    [[nodiscard]] inline std::optional<char> peak(int ahead=1) const //nodiscard : to indicate the return value should not be ignored
    {
        if(position+ahead > source.size())
        {
            return {};
        }
        else
        {
            return source[position];
        }
    }

    inline char consume() //function to consume the current character and advance the position
    {
        return source[position++];
    }


    const std::string source;
    size_t position = 0;

};