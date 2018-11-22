#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cctype>
#include <unordered_set>

#include <libs/module.h>
#include <constant.h>
#include <stringiter/stringiter.h>
#include <rules/rules.h>
#include <lookup/lookup.h>
#include <exception.h>
#include <lexer/lexer.h>

namespace lex = ws::lexer;

#include "callbacks.h"





constexpr lex::Rules handlers = {
    {
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_",
        ident_handler
    },

    {"\n\t\r ", whitespace_handler},
    {"0123456789", number_handler},
    {"|+-*/=<>^'", op_handler},

    {"{", left_brace_handler},
    {"}", right_brace_handler},

    {"[", left_bracket_handler},
    {"]", right_bracket_handler},

    {"(", left_paren_handler},
    {")", right_paren_handler},

    {",:.;", seperator_handler},
    {"\"", string_handler},
    {"#", comment_handler}
};












int main(int, char const*[]) {
    const auto start_total = std::chrono::high_resolution_clock::now();

    // Receive data.
    const auto start_recv = std::chrono::high_resolution_clock::now();

    auto data = ws::module::receive_all();
    lex::Group tokens;

    if (data.empty()) {
        ws::module::warnln_em("stream is empty.", " (make sure you pipe some input into the program)");

        return 1;
    }

    const auto end_recv = std::chrono::high_resolution_clock::now();






    // Run tokenizer.
    const auto start_lexer = std::chrono::high_resolution_clock::now();

    try {
        tokens = lex::lexer(handlers, data);

    } catch (const lex::exception::Fatal&) {
        ws::module::errorln("unrecoverable error!");
        return 1;
    }

    const auto end_lexer = std::chrono::high_resolution_clock::now();






    // Finished.
    const auto end_total = std::chrono::high_resolution_clock::now();






    // Util function
    auto fmt_num_str = [] (auto&& val) {
        auto num = std::to_string(val);
        int ins_pos = num.size() - 3;

        for (; ins_pos > 0; ins_pos -= 3)
            num.insert(static_cast<std::string::size_type>(ins_pos), ",");

        return num;
    };






    // Print tokens.
    if constexpr(lex::constant::PRINT_TOKENS) {
        auto bold  = ws::module::style::bold;
        auto reset = ws::module::style::reset;

        namespace bright = ws::module::colour::fg::bright;
        namespace normal = ws::module::colour::fg;
        namespace dim    = ws::module::colour::fg::dim;


        constexpr auto middle_column = 17;
        constexpr auto right_column  = 32;

        ws::module::print_lines(1);
        ws::module::println_h(
            normal::yellow,
            "    Content",
            ws::module::repeat(" ", middle_column - 2), "Type",
            ws::module::repeat(" ", right_column - 5), "Position"
        );
        ws::module::print_lines(1);


        for (const auto& tok: tokens) {
            const auto& [type, pos, content] = tok;



            auto line_str   = std::to_string(pos.line);
            auto column_str = std::to_string(pos.column);


            auto left_str   = std::string{content};
            auto middle_str = std::string{token_strings[type]};
            auto right_str  = line_str + ", " + column_str;



            if (type == TYPE_WHITESPACE) {
                std::string new_str;

                for (const auto& c: middle_str) {
                    if (c == '\n')
                        new_str += "\\n";

                    else
                        new_str += c;
                }

                middle_str = new_str;
            }



            bool left_trimmed = false;
            bool middle_trimmed = false;
            bool right_trimmed = false;



            auto left_size   = left_str.size()   + 2;
            auto middle_size = middle_str.size() + 2;



            if (left_str.size() > 15) {
                left_str = left_str.substr(0, 12);
                left_trimmed = true;
                left_size = left_str.size() + 4;
            }


            if (middle_str.size() > 25) {
                middle_str = middle_str.substr(0, 23);
                middle_trimmed = true;
                middle_size = middle_str.size() + 4;
            }


            if (right_str.size() > 15) {
                right_str = right_str.substr(0, 12);
                right_trimmed = true;
            }



            unsigned middle_padding_number = middle_column - left_size;
            unsigned right_padding_number  = right_column - middle_size;



            auto middle_padding = ws::module::repeat("·", middle_padding_number);
            auto right_padding = ws::module::repeat("·", right_padding_number);



            ws::module::print_h(" => ");


            if (left_trimmed)
                ws::module::print(
                    dim::grey, "\"", reset,
                    bright::green, bold, left_str, reset,
                    dim::grey, "\"...", reset
                );

            else
                ws::module::print(
                    dim::grey, "\"", reset,
                    bright::green, bold, left_str, reset,
                    dim::grey, "\" ", reset
                );


            ws::module::print(dim::black, middle_padding, reset);



            if (middle_trimmed)
                ws::module::print(
                    dim::grey, " <", reset,
                    bright::blue, middle_str,
                    dim::grey, ">...", reset
                );

            else
                ws::module::print(
                    dim::grey, " <", reset,
                    bright::blue, middle_str,
                    dim::grey, "> ", reset
                );


            ws::module::print(dim::black, right_padding, reset);



            if (right_trimmed)
                ws::module::print(
                    dim::grey, " (", reset,
                    right_str,
                    dim::grey, ")...", reset
                );

            else
                ws::module::print(
                    dim::grey, " (", reset,
                    right_str,
                    dim::grey, ")", reset
                );




            ws::module::print_lines(1);
        }
    }




    // Add some spacing under certain conditions
    if constexpr(
        lex::constant::PRINT_TOKENS && not lex::constant::ENABLE_VERBOSE
    ) {
        ws::module::print_lines(1);
        ws::module::println_h(ws::module::tabs(1), "Statistics");
        ws::module::print_lines(1);
    }




    // Normal stats.
    if constexpr(
        lex::constant::ENABLE_STATS and not lex::constant::ENABLE_VERBOSE
    ) {

        const auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_total - start_total).count();

        const auto lexer_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_lexer - start_lexer).count();

        const auto recv_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_recv - start_recv).count();



        auto bold  = ws::module::style::bold;
        auto reset = ws::module::style::reset;

        namespace bright = ws::module::colour::fg::bright;
        namespace normal = ws::module::colour::fg;
        namespace dim    = ws::module::colour::fg::dim;



        ws::module::print_h(" => ");

        ws::module::print(
            "(", dim::grey, "file", reset, ") ",
            normal::red, fmt_num_str(recv_time), "ms"
        );

        ws::module::print(" / ");

        ws::module::print(
            "(", dim::grey, "lexer", reset, ") ",
            bold, bright::yellow, fmt_num_str(lexer_time), "ms"
        );

        ws::module::print(" / ");

        ws::module::println(
            "(", dim::grey, "total", reset, ") ",
            bright::blue, fmt_num_str(total_time), "ms"
        );
    }










    // Verbose stats.
    if constexpr(
        lex::constant::ENABLE_STATS and lex::constant::ENABLE_VERBOSE
    ) {

        const auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_total - start_total).count();

        const auto lexer_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_lexer - start_lexer).count();

        const auto recv_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_recv - start_recv).count();


        ws::module::print_lines(1);
        ws::module::println_h(ws::module::tabs(1), "Statistics");
        ws::module::print_lines(1);

        ws::module::successln("File  => ", fmt_num_str(recv_time),  "ms");
        ws::module::successln("Lexer => ", fmt_num_str(lexer_time), "ms");
        ws::module::successln("Total => ", fmt_num_str(total_time), "ms");

        ws::module::print_lines(1);
        ws::module::noticeln("Input      => ", fmt_num_str(data.size()), " B");
        ws::module::noticeln("Tokens     => ", fmt_num_str(tokens.size()));
        ws::module::noticeln("Capacity   => ", fmt_num_str(tokens.capacity()));
        ws::module::noticeln("Difference => ", fmt_num_str(tokens.capacity() - tokens.size()));
        ws::module::print_lines(1);
    }


    return 0;
}