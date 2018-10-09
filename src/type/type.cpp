#include "type.h"


namespace ws::lexer {
    std::string Type::join() const {
        std::string t;

        for (const auto& x: types)
            t += (x + ".");

        return t.substr(0, t.size() - 1); // remove last dot.
    }
}