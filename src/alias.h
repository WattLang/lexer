#pragma once


#include <vector>
#include <libs/token.h>


namespace ws::lexer {
    struct StringIter;

    using Token    = ws::token::Token;
    using Group    = ws::token::Group;
    using Position = ws::token::Position;

    using WhilePred = bool(*const)(StringIter&, char);
    using Rule      = void(*)(StringIter&, Group&);
}