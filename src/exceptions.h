#pragma once


#include <string>
#include <stdexcept>


#define WS_LEXER_NEW_EXCEPTION_TYPE(name, default_msg) \
    class name: public std::runtime_error { \
        private: \
            std::string msg; \
        public: \
            name(const std::string& msg_): \
                std::runtime_error(msg_), \
                msg(msg_) \
            { \
            } \
            \
            name(): \
                std::runtime_error(default_msg), \
                msg(default_msg) \
            { \
            } \
            \
            std::string get_msg() const { return msg; } \
    };







namespace ws::lexer {
    WS_LEXER_NEW_EXCEPTION_TYPE(LexicalNotice, "Lexical notice!")
    WS_LEXER_NEW_EXCEPTION_TYPE(LexicalWarn, "Lexical warning!")
    WS_LEXER_NEW_EXCEPTION_TYPE(LexicalError, "Lexical error!")
    WS_LEXER_NEW_EXCEPTION_TYPE(LexicalInternalError, "Lexical internal error!")
}






#undef WS_LEXER_NEW_EXCEPTION_TYPE