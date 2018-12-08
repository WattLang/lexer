#pragma once


#include <stdexcept>
#include <string>

#include <constant.h>
#include <exception.h>
#include <rules/rules.h>
#include <stringiter/stringiter.h>

#include <libs/module.h>


namespace ws::lexer {
    Group lexer(const Rules& rules, const std::string& input) {

        ws::lexer::StringIter iter(input);
        Group tokens(input, 3.3); // Ratio of letters to tokens.


        try {
            while (not iter.is_end()) {
                try {
                    if (auto rule = rules.at(iter.peek()); rule) {
                        rule(iter, tokens);
                        iter.incr();


                    } else {
                        throw exception::InternalError(
                            "unexpected character '" + std::string{iter.ptr(), 1} + "'."
                        );
                    }


                // Non-fatal errors, print and continue.
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


        // Fatal errors, exit.
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


        return tokens;
    }
}