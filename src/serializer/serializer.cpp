#include "serializer.h"


namespace ws::lexer {
    void Serializer::print(const ws::token::Token& obj) {
        ws::token::serialize(std::cout, obj);
    }
}