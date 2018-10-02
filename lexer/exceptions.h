#pragma once


#include <stdexcept>


namespace ws::lexer {
    class LexicalFailure: public std::runtime_error {
        public:
            LexicalFailure(const std::string& msg): std::runtime_error(msg) {}
    };
}