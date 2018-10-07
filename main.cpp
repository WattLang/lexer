#include <iostream>
#include <unordered_set>
#include <chrono>
#include <string>
#include <unordered_map>


// Lexer stuff.
#include <macros.h>
#include <aliases/aliases.h>
#include <token/token.h>
#include <position/position.h>
#include <exceptions.h>
#include <code/code.h>
#include <run/run.h>
#include <utils/utils.h>
#include <libs/module.h>





namespace lex = ws::lexer;





namespace globals {
    lex::Position pos;
}






int main(int, char const *[]) {
    auto start = std::chrono::steady_clock::now();


    lex::Matcher match;  // Defines handler callbacks for characters.



    // Comments.
    ws::lexer::insert_case(match, "#",
        [&] (lex::Code& code, const std::string&) {
            WS_LOG_HEADING("Comment handler.")

            auto builder = code.read_while([&] (char c) {
                globals::pos.column++;
                return (c != '\n');
            });

            return lex::Token{};
        }
    );


    // Strings.
    ws::lexer::insert_case(match, "\"",
        [&] (lex::Code& code, const std::string&) {
            WS_LOG_HEADING("String handler.")

            auto current_column = globals::pos.column;

            code.next(); globals::pos.column++; // Skip opening quote.

            auto builder = code.read_while([&] (char c) {
                globals::pos.column++;

                // Remember, c is always ptr + 1.
                return (not (c == '"' and code.current() != '\\'));
            });

            code.next(); globals::pos.column++; // Skip ending quote.

            return lex::Token{
                {globals::pos.line, current_column},
                {"literal", "string"},
                builder
            };
        }
    );


    // Digits.
    ws::lexer::insert_case(match, "0123456789",
        [&] (lex::Code& code, const std::string&) {
            WS_LOG_HEADING("Digit handler.")

            auto current_column = globals::pos.column;
            const std::string valid_chars = "0123456789.";

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
    ws::lexer::insert_case(match, "|<>+-*/=",
        [&] (lex::Code& code, const std::string& chars) {
            WS_LOG_HEADING("Operator handler.")

            auto current_column = globals::pos.column;

            auto builder = code.read_while([&] (char c) {
                globals::pos.column++;
                return (chars.find(c) != std::string::npos);
            });

            static const std::unordered_map<std::string, std::string> operators = {
                {"+",  "plus"},
                {"-",  "minus"},
                {"*",  "multiply"},
                {"/",  "divide"},

                {"+=",  "plus_mutate"},
                {"-=",  "minus_mutate"},
                {"*=",  "multiply_mutate"},
                {"/=",  "divide_mutate"},

                {"<",  "less"},
                {">",  "greater"},

                {"<=", "less_or_equal"},
                {">=", "greater_or_equal"},
                {"==", "compare"},

                {"=",  "assign"},

                {"|>", "pipe"}
            };

            if (operators.find(builder) == operators.end()) {
                throw lex::LexicalError(
                    "ill-formed operator '" + builder + "' in program."
                );
            }


            return lex::Token{
                {globals::pos.line, current_column},
                {"operator", operators.at(builder)},
                builder
            };
        }
    );



    // Block.
    ws::lexer::insert_case(match, "({[",
        [&] (lex::Code& code, const std::string&) {
            WS_LOG_HEADING("Block handler.")

            auto current_column = globals::pos.column;
            auto chr = code.current();

            static const std::unordered_map<char, std::string> type = {
                {'(', "parenthesis"},
                {'{', "brace"},
                {'[', "bracket"}
            };

            globals::pos.column++;

            return lex::Token{
                {globals::pos.line, current_column},
                {type.at(chr), "left"},
                chr
            };
        }
    );

    ws::lexer::insert_case(match, ")}]",
        [&] (lex::Code& code, const std::string&) {
            WS_LOG_HEADING("Block handler.")

            auto current_column = globals::pos.column;
            auto chr = code.current();

            static const std::unordered_map<char, std::string> type = {
                {')', "parenthesis"},
                {'}', "brace"},
                {']', "bracket"}
            };

            globals::pos.column++;

            return lex::Token{
                {globals::pos.line, current_column},
                {type.at(chr), "right"},
                chr
            };
        }
    );




    // Identifiers.
    ws::lexer::insert_case(match, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_",
        [&] (lex::Code& code, const std::string&) {
            WS_LOG_HEADING("Idenifier handler.")


            auto current_column = globals::pos.column;
            const std::string valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789";

            auto builder = code.read_while([&] (char c) {
                globals::pos.column++;
                return (valid_chars.find(c) != std::string::npos);
            });

            static const std::unordered_set<std::string> reserved_idents = {
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
            WS_LOG_HEADING("Seperator handler.")

            auto current_column = globals::pos.column;
            auto chr = code.current();

            static const std::unordered_map<char, std::string> type = {
                {':', "semicolon"},
                {';', "colon"},
                {',', "comma"},
                {'.', "dot"}
            };

            globals::pos.column++;

            return lex::Token{
                {globals::pos.line, current_column},
                {"seperator", type.at(chr)},
                chr
            };
        }
    );






    // Input stream.
    auto contents = ws::module::receive_all(lex::BUFFER_SIZE);
    lex::Code code(contents);  // The input code.
    lex::TokenContainer tokens;




    auto position_string = [&] () {
        return std::string{
            " on line " +
            std::to_string(globals::pos.line) +
            ", column " +
            std::to_string(globals::pos.column) +
            ": "
        };
    };



    try {
        // Run the lexer on the code and pass in an error handler too.
        tokens = lex::run(code, match);

    } catch (const lex::LexicalNotice& e) {
        ws::module::errorln(
            "lexical notice occured", position_string(),
            ws::module::lines(1), ws::module::tabs(1),
            e.get_msg()
        );

        return 1;

    } catch (const lex::LexicalWarn& e) {
        ws::module::errorln(
            "lexical warn occured", position_string(),
            ws::module::lines(1), ws::module::tabs(1),
            e.get_msg()
        );

        return 2;

    } catch (const lex::LexicalError& e) {
        ws::module::errorln(
            "lexical error occured", position_string(),
            ws::module::lines(1), ws::module::tabs(1),
            e.get_msg()
        );

        return 3;

    } catch (const lex::LexicalInternalError& e) {
        ws::module::errorln(
            "internal error occured", position_string(),
            ws::module::lines(1), ws::module::tabs(1),
            e.get_msg()
        );

        return 4;
    }









    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();



    // Pipe the tokens inside the container.
    ws::module::pipe(tokens);


    WS_LEXER_DEBUG(ws::module::printer << ws::module::lines(ws::lexer::HEADING_LINE_GAP))
    ws::module::successln(ws::module::style::bold, "Finished!");



    // Stats.
    ws::module::printer << ws::module::lines(ws::lexer::HEADING_LINE_GAP);
    ws::module::successln(ws::module::style::bold, "Statistics");



    ws::module::successln(
        ws::module::tabs(1), "Tokens   ", tokens.size()
    );

    ws::module::successln(
        ws::module::tabs(1), "Duration ", duration, "ms"
    );


    return 0;
}