#include "lexer.h"

#include <stdexcept>
#include <string>
#include <chrono>

#include <constant.h>
#include <exception.h>

#include <stringiter/stringiter.h>

#include <libs/module.h>



// Adds commas to large numbers. e.g `123456789` -> `123,456,789`
template <typename T>
std::string num_to_formatted_str(T val) {
    auto num = std::to_string(val);
    int ins_pos = num.size() - 3;

    for (; ins_pos > 0; ins_pos -= 3)
        num.insert(static_cast<std::string::size_type>(ins_pos), ",");

    return num;
}





namespace ws::lexer {
    alias::Group lexer(
        const Rules& rules,
        const std::string& input
    ) {
        const auto start = std::chrono::high_resolution_clock::now();


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








        // Statistics.
        const auto end = std::chrono::high_resolution_clock::now();



        ws::module::run_if<
            constant::ENABLE_STATS && not constant::ENABLE_VERBOSE
        >([&] {
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


            ws::module::successln_h("lex time: ", duration, "ms!");
        });


        ws::module::run_if<
            constant::ENABLE_STATS && constant::ENABLE_VERBOSE
        >([&] {
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


            ws::module::noticeln_h("statistics");

            ws::module::print_tabs(1);
            ws::module::println_em("string", "       ", num_to_formatted_str(input.size()));

            ws::module::print_tabs(1);
            ws::module::println_em("capacity", "     ", num_to_formatted_str(tokens.capacity()));

            ws::module::print_tabs(1);
            ws::module::println_em("tokens", "       ", num_to_formatted_str(tokens.size()));


            ws::module::print_tabs(1);
            ws::module::println_em("diff", "         ", num_to_formatted_str(tokens.capacity() - tokens.size()));


            ws::module::print_tabs(1);
            ws::module::println_em("lex time", "     ", num_to_formatted_str(duration), "ms");
        });





        tokens.shrink_to_fit();

        return tokens;
    }
}