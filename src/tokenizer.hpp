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
            char ch = peek().value();

            if(std::isalpha(ch))
            {

                buf.push_back(consume());
                while(peek().has_value() && (std::isalnum(peek().value()) || peek().value() == '_'))
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
                else if(buf=="tell_me")
                {
                    tokens.push_back({.type=TokenType::tell_me});
                    buf.clear();
                    continue;
                }
                else if(buf=="dillusion")
                {
                    tokens.push_back({.type=TokenType::dillusion});
                    buf.clear();
                    continue;
                }
                else if(buf=="maybe")
                {
                    tokens.push_back({.type=TokenType::maybe});
                    buf.clear();
                    continue;
                }
                else if(buf=="ormaybe")
                {
                    tokens.push_back({.type=TokenType::ormaybe});
                    buf.clear();
                    continue;
                }
                else if(buf=="secret")
                {
                    while(peek().has_value() && peek().value() != '\n')
                    {
                        consume();
                    }
                    buf.clear();
                    continue;
                }
                else if(buf=="moveon")
                {
                    tokens.push_back({.type=TokenType::moveon});
                    buf.clear();
                    continue;
                }
                else if(buf=="wait")
                {
                    tokens.push_back({.type=TokenType::wait});
                    buf.clear();
                    continue;
                }
                else if(buf=="hide")
                {
                    buf.clear();
                    while(peek().has_value()) {
                         if(peek().value() == 'h' && 
                            peek(1).has_value() && peek(1).value() == 'i' &&
                            peek(2).has_value() && peek(2).value() == 'd' && 
                            peek(3).has_value() && peek(3).value() == 'e')
                         {
                             consume(); consume(); consume(); consume();
                             break;
                         }
                         consume();
                    }
                    continue;
                }
                else
                {
                    tokens.push_back({.type=TokenType::ident,.value=buf});
                    buf.clear();
                    continue;
                }
            }
            else if(std::isdigit(ch))
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
            else if(std::isspace(ch))
            {           
                consume();
                continue;
            }
            else
            {
                switch(ch)
                {
                    case '(':
                        consume();
                        tokens.push_back({.type=TokenType::open_paren});
                        break;
                    case ')':
                        consume();
                        tokens.push_back({.type=TokenType::close_paren});
                        break;
                    case ';':
                        consume();
                        tokens.push_back({.type=TokenType::semi});
                        break;
                    case '=':
                        consume();
                        if(peek().has_value() && peek().value() == '='){
                             consume();
                             tokens.push_back({.type=TokenType::eq_eq});
                        } else {
                            tokens.push_back({.type=TokenType::eq});
                        }
                        break;
                    case '!':
                        consume();
                         if(peek().has_value() && peek().value() == '='){
                             consume();
                             tokens.push_back({.type=TokenType::neq});
                        } else {
                            std::cerr<<"Unexpected character '!' (did you mean '!='?)"<<std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;
                    case '<':
                        consume();
                        if(peek().has_value() && peek().value() == '='){
                             consume();
                             tokens.push_back({.type=TokenType::lte});
                        } else {
                            tokens.push_back({.type=TokenType::lt});
                        }
                        break;
                     case '>':
                        consume();
                        if(peek().has_value() && peek().value() == '='){
                             consume();
                             tokens.push_back({.type=TokenType::gte});
                        } else {
                            tokens.push_back({.type=TokenType::gt});
                        }
                        break;
                    case '+':
                        consume();
                        tokens.push_back({.type=TokenType::plus});
                        break;
                    case '*':
                        consume();
                        tokens.push_back({.type=TokenType::mul});
                        break;
                    case '-':
                        consume();
                        tokens.push_back({.type=TokenType::sub});
                        break;
                    case '/':
                        consume();
                        tokens.push_back({.type=TokenType::div});
                        break;
                    case '{':
                        consume();
                        tokens.push_back({.type=TokenType::open_curly});
                        break;
                    case '}':
                        consume();
                        tokens.push_back({.type=TokenType::close_curly});
                        break;
                    case '"':
                        // opening quote
                        consume();
                        tokens.push_back({.type=TokenType::double_quotes});
                        {
                            std::string s;
                            while(peek().has_value() && peek().value() != '"'){
                                s.push_back(consume());
                            }
                            // push string literal token (content only)
                            tokens.push_back({.type=TokenType::string_lit, .value=s});
                            if(peek().has_value() && peek().value() == '"'){
                                consume(); // closing quote
                                tokens.push_back({.type=TokenType::double_quotes});
                            } else {
                                std::cerr<<"Unterminated string literal"<<std::endl;
                                exit(EXIT_FAILURE);
                            }
                        }
                        break;
                    
                    default:
                        std::cerr<<"you sucks! Unexpected character: '" << ch << "' (ASCII: " << (int)ch << ")"<<std::endl;
                        exit(EXIT_FAILURE);
                }
                continue;
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