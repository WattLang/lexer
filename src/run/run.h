#pragma once


#include <functional>
#include <aliases/aliases.h>
#include <exceptions.h>


namespace ws::lexer {
    TokenContainer run(
        ws::lexer::Code& code,
        ws::lexer::Matcher match,
        std::function<void(ws::lexer::LexicalFailure)> error_handler
    );
}