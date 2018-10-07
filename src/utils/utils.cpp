#include "utils.h"


namespace ws::lexer {
    void insert_case(
        ws::lexer::Matcher& matcher,
        const std::string& chars,
        const ws::lexer::Handler& handler
    ) {
        for (const auto& c: chars)
            matcher[c] = {chars, handler};
    }
}