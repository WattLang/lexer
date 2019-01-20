#pragma once


#include <vector>
#include <string_view>
#include <functional>
#include <libs/token.h>


namespace ws::lexer {
    struct StringIter;

    using Token    = ws::token::Token;
    using Group    = std::vector<Token>;
    using Position = ws::token::Position;


    using WhilePred = bool(*const)(StringIter&, const char);
    using Rule = void(*)(StringIter&, Group&);
}