#pragma once

#include <string>
#include <optional>

enum class TokenType {
    bye,
    int_lit,
    semi,
    open_paren,
    close_paren,
    ident,
    hope,
    eq
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};
