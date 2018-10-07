#include "code.h"


#include <macros.h>
#include <libs/module.h>


namespace ws::lexer {

    std::string Code::read_while(ws::lexer::ReadIfPred pred) {
        std::string builder;


        WS_LEXER_DEBUG(ws::module::noticeln(
            ws::module::tabs(2), "Start building..."
        ))


        while (true) {
            char c = current();
            builder += c;  // Append the character to the builder.

            WS_LEXER_DEBUG(ws::module::noticeln(
                ws::module::tabs(3), "Found '", c, "'."
            ))


            if (is_end())
                break;


            // If the next char is not valid, break and return builder.
            if (not pred(at(ptr + 1))) {
                WS_LEXER_DEBUG(ws::module::noticeln(
                    ws::module::tabs(2), "Finished."
                ))

                break;
            }


            next();
        }

        WS_LEXER_DEBUG(ws::module::noticeln(
            ws::module::tabs(2), "Return '", builder, "'."
        ))

        return builder;
    }

}