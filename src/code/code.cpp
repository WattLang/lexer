#include "code.h"


#include <macros.h>
#include <libs/module.h>


namespace ws::lexer {

    std::string Code::read_while(ws::lexer::ReadIfPred pred) {
        std::string builder;


        WS_LOG(ws::module::tabs(1) + "Building")


        while (true) {
            char c = current();
            builder += c;  // Append the character to the builder.


            WS_LOG(ws::module::tabs(2) + "Found '" + c + "'.")


            if (is_end())
                break;


            // If the next char is not valid, break and return builder.
            if (not pred(at(ptr + 1)))
                break;


            next();
        }

        WS_LOG(ws::module::tabs(1) + "Return '" + builder + "'.");

        return builder;
    }

}