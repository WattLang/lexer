#include <iostream>

#include "lexer/types.h"
#include "lexer/exceptions.h"
#include "lexer/code.h"
#include "lexer/lexer.h"
#include "lexer/utils.h"

#include "modules/module/module.h"
#include "modules/json/single_include/nlohmann/json.hpp"







ws::lexer::Code code("123 + 456");  // The input code.
ws::lexer::Matcher match;  // Defines handler callbacks for characters.






int main(int argc, char const *argv[]) {

    // Digits.
    insert_case(match, "0123456789",
        [] (ws::lexer::Code& c, const std::string& chars) -> std::optional<ws::lexer::Token> {
            auto builder = c.read_while([&] (char c) {
                return (chars.find(c) != std::string::npos);
            });

            return ws::lexer::Token{"number", builder};
        }
    );


    // Whitespace.
    insert_case(match, "\n\t ",
        [] (ws::lexer::Code& c, const std::string& chars) -> std::optional<ws::lexer::Token> {
            auto builder = c.read_while([&] (char c) {
                return (chars.find(c) != std::string::npos);
            });

            return {};
        }
    );


    // Operators.
    insert_case(match, "+-*/",
        [] (ws::lexer::Code& c, const std::string& chars) -> std::optional<ws::lexer::Token> {
            auto builder = c.read_while([&] (char c) {
                return (chars.find(c) != std::string::npos);
            });

            return ws::lexer::Token{"operator", builder};
        }
    );









    // Run the lexer on the code and pass in an error handler too.
    auto tokens = ws::lexer::lexer(code, match, [&] (const std::exception& e) {
        std::cerr << "Oh dear god!\n";
        throw ws::lexer::LexicalFailure("Unrecoverable error!");
    });


    // Print the tokens inside the container.
    std::cout << tokens << std::endl;


    return 0;
}