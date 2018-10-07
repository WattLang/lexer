#pragma once


#include <vector>
#include <string>


namespace ws::lexer {

    struct Type {
        std::vector<std::string> types;

        Type() {}
        Type(std::initializer_list<std::string> types_): types(types_) {}

        std::string join() const;
    };

}