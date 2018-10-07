#include "run.h"


#include <stdexcept>
#include <constants.h>
#include <macros.h>
#include <libs/module.h>
#include <code/code.h>


namespace ws::lexer {
    TokenContainer run(
        ws::lexer::Code& code,
        ws::lexer::Matcher match,
        std::function<void(ws::lexer::LexicalFailure)> error_handler
    ) {
        TokenContainer tokens;


        WS_LEXER_DEBUG(
            ws::module::noticeln("Commence lexing.");
            ws::module::printer << std::endl
        )


        while (true and code.size() > 0) {
            char c = code.current();


            // Check if current character has an associated handler.
            try {
                MatchEntry search;

                if (match.find(c) != match.end())
                    search = match.at(c);

                else
                    throw ws::lexer::LexicalFailure(
                        "stray '" + std::string{c} + "' in program."
                    );


                // Run the handler and pass in a reference to
                // the code instance and the charset.
                auto result = search.second(code, search.first);


                // Push the optionally returned token to the container.
                if (not result.is_empty) {
                    WS_LEXER_DEBUG(ws::module::noticeln(
                        ws::module::tabs(1), "Append token."
                    ))

                    tokens.push_back(result);

                }

                WS_LEXER_DEBUG(ws::module::printer << std::endl)


            } catch (const ws::lexer::LexicalFailure& e) {
                error_handler(e);
            }





            // If we are on the final character,
            // break so we don't get an exception from the next statement.
            if (code.is_end())
                break;


            // Advance to the next character.
            code.next();
        }


        WS_LEXER_DEBUG(ws::module::noticeln("Finished lexing."))


        return tokens;
    }
}