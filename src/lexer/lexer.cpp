#include "lexer.h"

#include <stdexcept>
#include <string>

#include <constant.h>
#include <exception.h>

#include <stringiter/stringiter.h>

#include <libs/module.h>






namespace ws::lexer {
    alias::Group lexer(
        const Rules& rules,
        const std::string& input
    ) {
        ws::lexer::StringIter iter(input);

        alias::Group tokens(input, 3.3);
        // 3.3 here represents the ratio of the number of input characters to the number of output tokens. this seems to be a good value in my testing.


        try {
            while (not iter.is_end()) {
                // check if rule is valid.
                try {
                    if (const auto& rule = rules.at(iter.peek()); not rule) {
                        throw exception::InternalError(
                            "unexpected character '" + std::string{iter.ptr()} + "'."
                        );


                    } else {
                        rule(iter, tokens);
                        iter.incr();
                    }


                // Catch any non-fatal errors and just continue.
                } catch (const exception::Notice& e) {
                    ws::module::noticeln(
                        "notice occured: ",
                        ws::module::lines(1), ws::module::tabs(1),
                        e.get_msg()
                    );


                } catch (const exception::Warn& e) {
                    ws::module::warnln(
                        "warn occured: ",
                        ws::module::lines(1), ws::module::tabs(1),
                        e.get_msg()
                    );
                }
            }


        // Fatal errors, cease lexing.
        } catch (const exception::Error& e) {
            ws::module::errorln(
                "error occured: ",
                ws::module::lines(1), ws::module::tabs(1),
                e.get_msg()
            );

            throw exception::Fatal(e.get_msg());


        } catch (const exception::InternalError& e) {
            ws::module::errorln(
                "error occured: ",
                ws::module::lines(1), ws::module::tabs(1),
                e.get_msg()
            );

            throw exception::Fatal(e.get_msg());
        }



        //tokens.shrink_to_fit();

        return tokens;
    }
}