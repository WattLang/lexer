#pragma once


#include <stdexcept>


namespace ws::lexer {
    class LexicalFailure: public std::runtime_error {
        public:
            std::string msg;
            LexicalFailure(const std::string& msg_): std::runtime_error(msg_), msg(msg_) {}
    };
}