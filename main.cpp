#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

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


    auto data = ws::module::receive_all();
    lex::alias::Group tokens;


    if (data.empty()) {
        ws::module::warnln_em("stream is empty.", " (make sure you pipe some input into the program)");

        return 1;
    }



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


    return 0;
}