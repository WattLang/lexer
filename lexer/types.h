#pragma once


#include <string>
#include <vector>
#include <functional>
#include <map>
#include <iostream>
#include <optional>


namespace ws::lexer {
    // Types.
    struct Token {
        std::string type, val;
    };

    struct Code;  // Forward declare.

    using ReadIfPred = std::function<bool(char)>;
    using Handler    = std::function<std::optional<Token>(Code&, const std::string&)>;

    using TokenContainer = std::vector<Token>;

    using MatchEntry = std::pair<std::string, Handler>;
    using Matcher    = std::map<char, MatchEntry>;
}



namespace ws::lexer {
    // Print Token.
    std::ostream& operator<<(std::ostream& os, const ws::lexer::Token& tok) {
        os << "{\"" << tok.type << "\", \"" << tok.val << "\"}";
        return os;
    }


    // Print TokenContainer.
    std::ostream& operator<<(
        std::ostream& os,
        const ws::lexer::TokenContainer& toks
    ) {
        if (toks.empty())
            return (os << "[]");

        os << "[\n";

        for (auto it = toks.begin(); it != toks.end() - 1; ++it)
            os << '\t' << *it << ", \n";

        return (os << '\t' << toks.back() << "\n]");
    }
}