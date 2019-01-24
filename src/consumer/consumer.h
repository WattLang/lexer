#pragma once


#include <string>
#include <optional>

#include "../constant.h"
#include "../exception.h"
#include "../rules/rules.h"
#include "../stringiter/stringiter.h"

// #include <libs/logger.h>


namespace ws::lexer {

    std::optional<Token> get_next_token(
        const Rules& rules,
        ws::lexer::StringIter& iter,
        ws::lexer::Group& tokens
    ) {
        auto size = tokens.size();

        rules.at(iter.peek()) (iter, tokens);
        iter.incr();

        if (size != tokens.size())
            return tokens.back();

        return {};
    }


    void consume(
        const Rules& rules,
        ws::lexer::StringIter& iter,
        ws::lexer::Group& tokens
    ) {
        rules.at(iter.peek()) (iter, tokens);
        iter.incr();
    }


    void lexer(
        const Rules& rules,
        ws::lexer::StringIter& iter,
        ws::lexer::Group& tokens
    ) {
        while (not iter.is_end())
            consume(rules, iter, tokens);
    }

}
