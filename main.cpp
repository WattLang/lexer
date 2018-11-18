#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cctype>
#include <unordered_set>
#include <set>

#include <libs/module.h>
#include <constant.h>
#include <stringiter/stringiter.h>
#include <rules/rules.h>
#include <lookup/lookup.h>
#include <exception.h>
#include <lexer/lexer.h>

namespace lex = ws::lexer;

#include "callbacks.h"






int main(int, char const*[]) {
    const auto start = std::chrono::high_resolution_clock::now();


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



    const auto start_recv = std::chrono::high_resolution_clock::now();

    auto data = ws::module::receive_all();
    lex::alias::Group tokens;


    if (data.empty()) {
        ws::module::warnln_em("stream is empty.", " (make sure you pipe some input into the program)");

        return 1;
    }

    const auto end_recv = std::chrono::high_resolution_clock::now();



    try {
        tokens = lex::lexer(handlers, data);

    } catch (const lex::exception::Fatal&) {
        ws::module::errorln("unrecoverable error!");
        return 1;
    }


    ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
        for (const auto& tok: tokens)
            ws::module::println(tok);
    });



    const auto end = std::chrono::high_resolution_clock::now();

    ws::module::run_if<
        lex::constant::ENABLE_STATS && not lex::constant::ENABLE_VERBOSE
    >([&] {
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        ws::module::successln_h("tot time: ", duration, "ms!");


        const auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end_recv - start_recv).count();

        ws::module::successln_h("rec time: ", duration2, "ms!");
    });



    ws::module::run_if<
        lex::constant::ENABLE_STATS && lex::constant::ENABLE_VERBOSE
    >([&] {
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        ws::module::print_tabs(1);
        ws::module::println_em("total time", "   ", duration, "ms");
    });


    return 0;
}