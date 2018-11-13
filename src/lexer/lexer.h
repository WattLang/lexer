#pragma once


#include <string>

#include <rules/rules.h>
#include <alias.h>


namespace ws::lexer {

    alias::TokenGroup lexer(const Rules& rules, const std::string& input);

}