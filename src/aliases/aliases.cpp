#include "aliases.h"


namespace ws::lexer {
    using json = nlohmann::json;  // less verbose

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
        json j = json::array();

        for (const auto& tok: toks)
            j.emplace_back(tok.as_json());

        return (os << j.dump(ws::lexer::INDENT_SIZE));
    }
}