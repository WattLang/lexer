#pragma once


#include <string>
#include <aliases/aliases.h>


namespace ws::lexer {
    // Insert handlers into match map based on string of chars.
    void insert_case(
        ws::lexer::Matcher& matcher,
        const std::string& chars,
        const ws::lexer::Handler& handler
    );
}