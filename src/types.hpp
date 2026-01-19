#pragma once

#include <string>
#include <optional>

enum class TokenType {
    bye,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};
