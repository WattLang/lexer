#pragma once


#include <vector>
#include <libs/token.h>


namespace ws::lexer {
    // Forward declarations.
    struct Serializer;
    struct StringIter;
    struct Rules;
}


namespace ws::lexer::alias {
    using TokenGroup = std::vector<ws::token::Token>;


    using WhilePred = bool(*)(StringIter&, char);
    using Rule = void(*)(StringIter&, TokenGroup&);
}