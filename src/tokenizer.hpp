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
            int line = m_line;
            int col = m_col;
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
                    tokens.push_back({.type=TokenType::bye, .line=line, .col=col});
                    buf.clear();
                    continue;
                }
                else if(buf=="hope")
                {
                    tokens.push_back({.type=TokenType::hope, .line=line, .col=col});
                    buf.clear();
                    continue;
                }
                else if(buf=="tell_me")
                {
                    tokens.push_back({.type=TokenType::tell_me, .line=line, .col=col});
                    buf.clear();
                    continue;
                }
                else if(buf=="dillusion")
                {
                    tokens.push_back({.type=TokenType::dillusion, .line=line, .col=col});
                    buf.clear();
                    continue;
                }
                else if(buf=="maybe")
                {
                    tokens.push_back({.type=TokenType::maybe, .line=line, .col=col});
                    buf.clear();
                    continue;
                }
                else if(buf=="ormaybe")
                {
                    tokens.push_back({.type=TokenType::ormaybe, .line=line, .col=col});
                    buf.clear();
                    continue;
                }
                else if(buf=="then")
                {
                    tokens.push_back({.type=TokenType::then_tok, .line=line, .col=col});
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
                    tokens.push_back({.type=TokenType::moveon, .line=line, .col=col});
                    buf.clear();
                    continue;
                }
                else if(buf=="wait")
                {
                    tokens.push_back({.type=TokenType::wait, .line=line, .col=col});
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
                    tokens.push_back({.type=TokenType::ident, .line=line, .col=col, .value=buf});
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
                tokens.push_back({.type=TokenType::int_lit, .line=line, .col=col, .value=buf});
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
                        tokens.push_back({.type=TokenType::open_paren, .line=line, .col=col});
                        break;
                    case ')':
                        consume();
                        tokens.push_back({.type=TokenType::close_paren, .line=line, .col=col});
                        break;
                    case ';':
                        consume();
                        tokens.push_back({.type=TokenType::semi, .line=line, .col=col});
                        break;
                    case ',':
                        consume();
                        tokens.push_back({.type=TokenType::comma, .line=line, .col=col});
                        break;
                    case '=':
                        consume();
                        if(peek().has_value() && peek().value() == '='){
                             consume();
                             tokens.push_back({.type=TokenType::eq_eq, .line=line, .col=col});
                        } else {
                            tokens.push_back({.type=TokenType::eq, .line=line, .col=col});
                        }
                        break;
                    case '!':
                        consume();
                         if(peek().has_value() && peek().value() == '='){
                             consume();
                             tokens.push_back({.type=TokenType::neq, .line=line, .col=col});
                        } else {
                            std::cerr << "Line " << line << ":" << col << " Unexpected character '!' (did you mean '!='?)" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;
                    case '<':
                        consume();
                        if(peek().has_value() && peek().value() == '='){
                             consume();
                             tokens.push_back({.type=TokenType::lte, .line=line, .col=col});
                        } else {
                            tokens.push_back({.type=TokenType::lt, .line=line, .col=col});
                        }
                        break;
                     case '>':
                        consume();
                        if(peek().has_value() && peek().value() == '='){
                             consume();
                             tokens.push_back({.type=TokenType::gte, .line=line, .col=col});
                        } else {
                            tokens.push_back({.type=TokenType::gt, .line=line, .col=col});
                        }
                        break;
                    case '+':
                        consume();
                        tokens.push_back({.type=TokenType::plus, .line=line, .col=col});
                        break;
                    case '*':
                        consume();
                        tokens.push_back({.type=TokenType::mul, .line=line, .col=col});
                        break;
                    case '-':
                        consume();
                        tokens.push_back({.type=TokenType::sub, .line=line, .col=col});
                        break;
                    case '/':
                        consume();
                        tokens.push_back({.type=TokenType::div, .line=line, .col=col});
                        break;
                    case '{':
                        consume();
                        tokens.push_back({.type=TokenType::open_curly, .line=line, .col=col});
                        break;
                    case '}':
                        consume();
                        tokens.push_back({.type=TokenType::close_curly, .line=line, .col=col});
                        break;
                    case '"':
                        // opening quote
                        consume();
                        tokens.push_back({.type=TokenType::double_quotes, .line=line, .col=col});
                        {
                            std::string s;
                            while(peek().has_value() && peek().value() != '"'){
                                s.push_back(consume());
                            }
                            // push string literal token (content only)
                            tokens.push_back({.type=TokenType::string_lit, .line=line, .col=col, .value=s});
                            if(peek().has_value() && peek().value() == '"'){
                                consume(); // closing quote
                                tokens.push_back({.type=TokenType::double_quotes, .line=line, .col=col});
                            } else {
                                std::cerr << "Line " << line << ":" << col << " Unterminated string literal" << std::endl;
                                exit(EXIT_FAILURE);
                            }
                        }
                        break;
                    
                    default:
                        std::cerr << "Line " << line << ":" << col << " you sucks! Unexpected character: '" << ch << "' (ASCII: " << (int)ch << ")" << std::endl;
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
        char c = source[position++];
        if(c == '\n') {
            m_line++;
            m_col = 1;
        } else {
            m_col++;
        }
        return c;
    }


    const std::string source;
    size_t position = 0;
    int m_line = 1;
    int m_col = 1;

};