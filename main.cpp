#include <iostream>
#include <iomanip>
#include <locale>
#include <fstream>
#include <chrono>
#include <string>
#include <cctype>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <map>

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
    {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_", on_ident},

    {" \f\r\t\v", on_whitespace},
    {"\n", on_newline},

    {"0123456789", on_number},
    {"+-*/=<>", on_op},

    {":", on_colon},
    {";", on_semicolon},
    {".", on_dot},
    {",", on_comma},

    {"\"", on_string},

    {"(", on_left_paren},
    {")", on_right_paren},

    {"[", on_left_brace},
    {"]", on_right_brace},

    {"{", on_left_bracket},
    {"}", on_right_bracket}
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







    ws::module::printer.setf(std::ios::fixed);
    ws::module::printer.setf(std::ios::showpoint);
    ws::module::printer.precision(3);
    ws::module::printer.imbue(std::locale(""));






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
            normal::red, recv_time, "ms"
        );

        ws::module::print(" / ");

        ws::module::print(
            "(", dim::grey, "lexer", reset, ") ",
            bold, bright::yellow, lexer_time, "ms"
        );

        ws::module::print(" / ");

        ws::module::println(
            "(", dim::grey, "total", reset, ") ",
            bright::blue, total_time, "ms"
        );
    }










    // Verbose stats.
    if constexpr(
        lex::constant::ENABLE_STATS and lex::constant::ENABLE_VERBOSE
    ) {

        const auto total_time_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(end_total - start_total).count();

        const auto lexer_time_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(end_lexer - start_lexer).count();

        const auto recv_time_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(end_recv - start_recv).count();


        const double total_time_milli = total_time_nano / 1000 / 1000;
        const double lexer_time_milli = lexer_time_nano / 1000 / 1000;
        const double recv_time_milli  = recv_time_nano  / 1000 / 1000;


        const double total_time_sec = total_time_milli / 100;
        const double lexer_time_sec = lexer_time_milli / 100;
        const double recv_time_sec  = recv_time_milli  / 100;


        const double input_size_mb = static_cast<double>(data.size()) / 1024 / 1024;

        const double bandwidth = input_size_mb / lexer_time_sec;

        const double ln_per_ms = static_cast<double>(CURRENT_POSITION.line) / lexer_time_milli;

        const double chr_per_ms = static_cast<double>(data.size()) / lexer_time_milli;




        ws::module::print_lines(1);
        ws::module::println_h(ws::module::tabs(1), "Statistics");
        ws::module::print_lines(1);



        ws::module::warnln("Throughput => ", bandwidth,  " MB/s");
        ws::module::warnln("Lines      => ", ln_per_ms,  " ln/ms");
        ws::module::warnln("Chars      => ", chr_per_ms, " chr/ms");


        ws::module::print_lines(1);
        ws::module::successln("File  => ", recv_time_milli,  "ms");
        ws::module::successln("Lexer => ", lexer_time_milli, "ms");
        ws::module::successln("Total => ", total_time_milli, "ms");

        ws::module::print_lines(1);
        ws::module::noticeln("Input      => ", data.size(), " B");
        ws::module::noticeln("Tokens     => ", tokens.size());
        ws::module::noticeln("Capacity   => ", tokens.capacity());
        ws::module::noticeln("Difference => ", tokens.capacity() - tokens.size());
        ws::module::print_lines(1);
    }


    return 0;
}