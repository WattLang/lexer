#pragma once


#include <functional>
#include <stdexcept>
#include "types.h"


namespace ws::lexer {
    inline TokenContainer lexer(
        Code& code,
        Matcher match,
        std::function<void(std::exception)> error_handler
    ) {
        TokenContainer tokens;


        while (true) {
            char c = code.current();


            // Check if current character has an associated handler.
            try {
                MatchEntry search = match.at(c);


                // Run the handler and pass in a reference to
                // the code instance and the charset.
                auto result = search.second(code, search.first);


                // Push the optionally returned token to the container.
                if (result)
                    tokens.push_back(result.value());

            } catch (const std::exception& e) {
                error_handler(e);

            }





            // If we are on the final character,
            // break so we don't get an exception from the next statement.
            if (code.is_end())
                break;


            // Advance to the next character.
            code.next();
        }


        return tokens;
    }
}