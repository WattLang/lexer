#include <iostream>
#include <set>

#include "src/macros.h"
#include "src/aliases/aliases.h"
#include "src/exceptions.h"
#include "src/code/code.h"
#include "src/run/run.h"
#include "src/utils/utils.h"
#include "src/libs/module.h"



namespace lex = ws::lexer;
using json = nlohmann::json;





namespace globals {
    lex::Position pos;
}





int main(int, char const *[]) {
    lex::Matcher match;  // Defines handler callbacks for characters.



    // Digits.
    ws::lexer::insert_case(match, "0123456789",
        [&] (lex::Code& code, const std::string&) {
            WS_LEXER_DEBUG(ws::module::noticeln(
                ws::module::tabs(1), ws::module::style::bold, "Digit handler."
            ))

            auto current_column = globals::pos.column;
            std::string valid_chars = "0123456789.";

            auto builder = code.read_while([&] (char c) {
                globals::pos.column++;
                return (valid_chars.find(c) != std::string::npos);
            });

            return lex::Token{
                {globals::pos.line, current_column},
                {"literal", "number"},
                builder
            };
        }
    );


    // Whitespace.
    ws::lexer::insert_case(match, "\n\t ",
        [&] (lex::Code& code, const std::string&) {
            WS_LEXER_DEBUG(ws::module::noticeln(
                ws::module::tabs(1), ws::module::style::bold, "Whitespace handler."
            ))

            auto chr = code.current();

            if (chr == '\n') {
                globals::pos.line++;
                globals::pos.column = 1;

            } else {
                globals::pos.column++;
            }

            return lex::Token{};
        }
    );


    // Operators.
    ws::lexer::insert_case(match, "<>+-*/=",
        [&] (lex::Code& code, const std::string& chars) {
            WS_LEXER_DEBUG(ws::module::noticeln(
                ws::module::tabs(1), ws::module::style::bold, "Operator handler."
            ))

            auto current_column = globals::pos.column;

            auto builder = code.read_while([&] (char c) {
                globals::pos.column++;
                return (chars.find(c) != std::string::npos);
            });

            static std::map<std::string, std::string> operators = {
                {"+",  "plus"},
                {"-",  "minus"},
                {"*",  "multiply"},
                {"/",  "divide"},

                {"<",  "less"},
                {">",  "greater"},

                {"<=", "less_or_equal"},
                {">=", "greater_or_equal"},
                {"==", "compare"},

                {"=",  "assign"}
            };

            if (operators.find(builder) == operators.end()) {
                throw lex::LexicalFailure(
                    "Unknown operator: '" + builder + "'!"
                );
            }


            return lex::Token{
                {globals::pos.line, current_column},
                {"operator", operators[builder]},
                builder
            };
        }
    );



    // Block.
    ws::lexer::insert_case(match, "({[",
        [&] (lex::Code& code, const std::string&) {
            WS_LEXER_DEBUG(ws::module::noticeln(
                ws::module::tabs(1), ws::module::style::bold, "Block handler."
            ))

            auto chr = code.current();

            static std::map<char, std::string> type = {
                {'(', "parenthesis"},
                {'{', "brace"},
                {'[', "bracket"}
            };

            globals::pos.column++;

            return lex::Token{
                globals::pos,
                {type[chr], "left"},
                chr
            };
        }
    );

    ws::lexer::insert_case(match, ")}]",
        [&] (lex::Code& code, const std::string&) {
            WS_LEXER_DEBUG(ws::module::noticeln(
                ws::module::tabs(1), ws::module::style::bold, "Block handler."
            ))

            auto chr = code.current();

            static std::map<char, std::string> type = {
                {')', "parenthesis"},
                {'}', "brace"},
                {']', "bracket"}
            };

            globals::pos.column++;

            return lex::Token{
                globals::pos,
                {type[chr], "right"},
                chr
            };
        }
    );




    // Identifiers.
    ws::lexer::insert_case(match, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_",
        [&] (lex::Code& code, const std::string&) {
            WS_LEXER_DEBUG(ws::module::noticeln(
                ws::module::tabs(1), ws::module::style::bold, "Identifier handler."
            ))

            auto current_column = globals::pos.column;
            std::string valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789";

            auto builder = code.read_while([&] (char c) {
                globals::pos.column++;
                return (valid_chars.find(c) != std::string::npos);
            });

            static std::set<std::string> reserved_idents = {
                "if", "else",
                "while", "for", "loop",
                "struct",
                "true", "false",
                "namespace",
                "let",
                "int", "float", "byte", "str"
            };

            if (reserved_idents.find(builder) == reserved_idents.end()) {
                return lex::Token{
                    {globals::pos.line, current_column},
                    {"identifier"},
                    builder
                };

            } else {
                return lex::Token{
                    {globals::pos.line, current_column},
                    {"keyword", builder},
                    builder
                };
            }
        }
    );




    // Seperators
    ws::lexer::insert_case(match, ":;,.",
        [&] (lex::Code& code, const std::string&) {
            WS_LEXER_DEBUG(ws::module::noticeln(
                ws::module::tabs(1), ws::module::style::bold, "Seperator handler."
            ))

            auto chr = code.current();

            static std::map<char, std::string> type = {
                {':', "semicolon"},
                {';', "colon"},
                {',', "comma"},
                {'.', "dot"}
            };

            globals::pos.column++;

            return lex::Token{
                globals::pos,
                {"seperator", type[chr]},
                chr
            };
        }
    );



















    // Input stream.
    auto contents = ws::module::receive_all(lex::BUFFER_SIZE);
    lex::Code code(contents);  // The input code.
    lex::TokenContainer tokens;





    try {
        // Run the lexer on the code and pass in an error handler too.
        tokens = lex::run(code, match,
            [&] (const lex::LexicalFailure& e) {
                ws::module::errorln(
                    "lexical error on line ", globals::pos.line,
                    ", column ", globals::pos.column, ":",
                    ws::module::lines(1), ws::module::tabs(1),
                    e.msg
                );

                throw lex::LexicalFailure("error occured!");
            }
        );

    } catch (const lex::LexicalFailure&) {
        return 1;  // There was an error, so return non-zero
    }



    // Pipe the tokens inside the container.
    ws::module::pipe(tokens);


    ws::module::successln("Finished!");


    return 0;
}