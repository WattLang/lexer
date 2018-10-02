#include <iostream>

#include "lexer/constants.h"
#include "lexer/types.h"
#include "lexer/exceptions.h"
#include "lexer/code.h"
#include "lexer/lexer.h"
#include "lexer/utils.h"
#include "lexer/libs.h"



namespace lex = ws::lexer;
using json = nlohmann::json;












int main(int argc, char const *argv[]) {
    // Input stream.
    auto contents = ws::module::receive_all(lex::BUFFER_SIZE);




    lex::Code code(contents);  // The input code.
    lex::Matcher match;  // Defines handler callbacks for characters.
    lex::TokenContainer tokens;



    // Digits.
    insert_case(match, "0123456789",
        [] (lex::Code& c, const std::string& chars) -> std::optional<lex::Token> {
            std::string valid_chars = "0123456789.";

            auto builder = c.read_while([&] (char c) {
                return (valid_chars.find(c) != std::string::npos);
            });

            return lex::Token{"number", builder};
        }
    );


    // Whitespace.
    insert_case(match, "\n\t ",
        [] (lex::Code& c, const std::string& chars) -> std::optional<lex::Token> {
            auto builder = c.read_while([&] (char c) {
                return (chars.find(c) != std::string::npos);
            });

            return {};
        }
    );


    // Operators.
    insert_case(match, "+-*/",
        [] (lex::Code& c, const std::string& chars) -> std::optional<lex::Token> {
            auto builder = c.read_while([&] (char c) {
                return (chars.find(c) != std::string::npos);
            });

            return lex::Token{"operator", builder};
        }
    );


    // Scopes.
    insert_case(match, "{}[]()",
        [] (lex::Code& c, const std::string& chars) -> std::optional<lex::Token> {
            auto builder = c.read_while([&] (char c) {
                return (chars.find(c) != std::string::npos);
            });

            return lex::Token{"scope", builder};
        }
    );






    try {
        // Run the lexer on the code and pass in an error handler too.
        tokens = lex::lexer(code, match, [&] (const std::exception& e) {
            ws::module::errorln("Invalid character: '", code.current(), "'!");
            ws::module::errorln(ws::module::tabs(1), "Terminating...");

            throw lex::LexicalFailure("Error!");
        });

    } catch (const lex::LexicalFailure&) {
        return 1;
    }


    // Pipe the tokens inside the container.
    ws::module::pipe(tokens);


    return 0;
}