#include "token.h"
#include <constants.h>


namespace ws::lexer {


    Token::Token(): is_empty(true) {}


    Token::Token(
        const ws::lexer::Position& pos_,
        const ws::lexer::Type& type_,
        const std::string& content_
    ):
        pos(pos_),
        type(type_),
        content(content_),
        is_empty(false)
    {

    }


    Token::Token(
        const ws::lexer::Position& pos_,
        const ws::lexer::Type& type_,
        char content_
    ):
        pos(pos_),
        type(type_),
        content(std::string{content_}),
        is_empty(false)
    {

    }


    json Token::as_json() const {
        return {
            {"line", pos.line},
            {"column", pos.column},
            {"type", type.join()},
            {"content", content}
        };
    }


    std::string Token::as_string() const {
        return as_json().dump(ws::lexer::INDENT_SIZE);
    }

}