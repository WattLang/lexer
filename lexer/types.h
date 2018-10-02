#pragma once


#include <string>
#include <vector>
#include <functional>
#include <map>
#include <iostream>
#include <optional>

#include "constants.h"
#include "libs.h"


namespace ws::lexer {
    using json = nlohmann::json;  // less verbose


    // Types.
    struct Token {
        std::string type, val;


        json as_json() const {
            return {
                {"type", type},
                {"val", val}
            };
        }


        std::string as_string() const {
            return as_json().dump(ws::lexer::INDENT_SIZE);
        }
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
        os << tok.as_string();
        return os;
    }


    // Print TokenContainer.
    std::ostream& operator<<(
        std::ostream& os,
        const ws::lexer::TokenContainer& toks
    ) {
        json j;

        for (const auto& tok: toks)
            j.emplace_back(tok.as_json());

        return (os << j.dump(ws::lexer::INDENT_SIZE));
    }
}