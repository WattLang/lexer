#pragma once


#include <vector>
#include <libs/token.h>


namespace ws::lexer {
    // Forward declarations.
    struct StringIter;
    struct Rules;
}


namespace ws::lexer::alias {
    using Token = ws::token::Token;
    using Group = ws::token::Group;


    using WhilePred = bool(*)(StringIter&, char);
    using Rule = void(*)(StringIter&, Group&);
}