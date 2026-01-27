#pragma once

#include <string>
#include <optional>

enum class TokenType {
    bye, // exit
    int_lit,
    semi,
    open_paren,
    close_paren,
    ident,
    hope, //int variable declaration
    eq,
    plus,
    mul,
    sub,
    div,
    tell_me,
    string_lit,
    double_quotes,
    dillusion // string literal
};

bool is_bin_op(TokenType type){
    switch(type){
        case TokenType::plus:
        case TokenType::mul:
        case TokenType::sub:
        case TokenType::div:
            return true;
        default:
            return false;
    }
}


std::optional<int> bin_prec(TokenType type){
    switch(type){
        case TokenType::plus:
            return 0;
        case TokenType::mul:
            return 1;
        case TokenType::sub:
            return 0;
        case TokenType::div:
            return 1;
        default:
            return {};
    }
}



struct Token {
    TokenType type;
    std::optional<std::string> value {};
};
