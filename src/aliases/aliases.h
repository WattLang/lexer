#pragma once


#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <iostream>


#include <libs/json.h>
#include <constants.h>
#include <token/token.h>


namespace ws::lexer {
    struct Code;  // Forward declare.


    using ReadIfPred = std::function<bool(char)>;
    using Handler    = std::function<
        ws::lexer::Token(ws::lexer::Code&, const std::string&)
    >;

    using TokenContainer = std::vector<ws::lexer::Token>;

    using MatchEntry = std::pair<std::string, Handler>;
    using Matcher    = std::unordered_map<char, ws::lexer::MatchEntry>;
}



namespace ws::lexer {
    std::ostream& operator<<(std::ostream& os, const ws::lexer::Token& tok);

    std::ostream& operator<<(
        std::ostream& os,
        const ws::lexer::TokenContainer& toks
    );
}