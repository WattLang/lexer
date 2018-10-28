#pragma once


#include <iostream>
#include <libs/token.h>


namespace ws::lexer {
    struct Serializer {
        void print(const ws::token::Token& obj);
    };
}