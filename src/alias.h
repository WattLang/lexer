#pragma once


#include <vector>

#include <libs/token.h>


namespace ws::lexer {
    // Forward declarations.
    struct Serializer;
    struct StringIter;
    struct HandlerGroup;
}

namespace ws::lexer::alias {
    using TokenGroup = std::vector<ws::token::Token>;


    using WhilePred = bool(*)(StringIter&, char);
    using Handler = void(*)(StringIter&, Serializer&);


    //using WhilePred = std::function<bool(char)>;
    //using Handler = std::function<Token(StringIter&)>;
}