#include "run.h"


#include <stdexcept>
#include <constants.h>
#include <macros.h>
#include <libs/module.h>
#include <code/code.h>


namespace ws::lexer {
    TokenContainer run(
        ws::lexer::Code& code,
        ws::lexer::Matcher match
    ) {
        TokenContainer tokens;


        while (true and code.size() > 0) {
            char c = code.current();


            // Check if current character has an associated handler.
            MatchEntry search;

            if (match.find(c) != match.end())
                search = match.at(c);

            else
                throw ws::lexer::LexicalInternalError(
                    "stray '" + std::string{c} + "' in program."
                );


            // Run the handler and pass in a reference to
            // the code instance and the charset.
            auto result = search.second(code, search.first);


            // Push the optionally returned token to the container.
            if (not result.is_empty) {
                WS_LOG("Append token.")
                tokens.push_back(result);
            }





            // If we are on the final character,
            // break so we don't get an exception from the next statement.
            if (code.is_end())
                break;


            code.next();
        }


        return tokens;
    }
}