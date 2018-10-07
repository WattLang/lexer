#pragma once


#include <vector>
#include <string>

#include <libs/json.h>
#include <type/type.h>
#include <position/position.h>



namespace ws::lexer {
    using json = nlohmann::json;  // less verbose


    struct Token {
        ws::lexer::Position pos;
        ws::lexer::Type type;
        std::string content;

        bool is_empty = true;


        Token();


        Token(
            const ws::lexer::Position& pos_,
            const ws::lexer::Type& type_,
            const std::string& content_
        );


        Token(
            const ws::lexer::Position& pos_,
            const ws::lexer::Type& type_,
            char content_
        );

        json as_json() const;
        std::string as_string() const;
    };

}