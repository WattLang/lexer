#pragma once



using Group = lex::alias::Group;
using StringIter = lex::StringIter;




enum: ws::token::Type {
    TYPE_NULL,
    TYPE_IDENTIFIER,
    TYPE_KEYWORD,
    TYPE_STRING,
    TYPE_SEPERATOR,
    TYPE_COMMENT,

    TYPE_FUNC_DEF,

    TYPE_OPERATOR,

    TYPE_OPERATOR_RAISE,
    TYPE_OPERATOR_ARROW,
    TYPE_OPERATOR_QUOTE,

    TYPE_OPERATOR_INCR,
    TYPE_OPERATOR_DECR,

    TYPE_OPERATOR_PLUS_EQ,
    TYPE_OPERATOR_PLUS,

    TYPE_OPERATOR_LESS_EQ,
    TYPE_OPERATOR_LESS,

    TYPE_OPERATOR_MORE_EQ,
    TYPE_OPERATOR_MORE,

    TYPE_OPERATOR_MINUS_EQ,
    TYPE_OPERATOR_MINUS,

    TYPE_OPERATOR_DIV_EQ,
    TYPE_OPERATOR_DIV,

    TYPE_OPERATOR_MUL_EQ,
    TYPE_OPERATOR_MUL,

    TYPE_OPERATOR_ASSIGN,
    TYPE_OPERATOR_COMPARE,

    TYPE_OPERATOR_PIPE,

    TYPE_PAREN_LEFT,
    TYPE_PAREN_RIGHT,

    TYPE_BRACKET_LEFT,
    TYPE_BRACKET_RIGHT,

    TYPE_BRACE_LEFT,
    TYPE_BRACE_RIGHT,

    TYPE_LITERAL_FLOAT
};























constexpr lex::LookupTable ident_table = {
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", true
};

const std::unordered_set<std::string> keywords = {
    "var", "struct", "self",
    "if", "else", "for", "while",
    "break", "continue", "return",
    "nil", "true", "false"
};

void ident_handler(StringIter& iter, Group& tokens) {
    const auto builder = iter.read_while([] (auto&, char c) {
        return ident_table[c];
    });

    if (keywords.find(std::string{builder}) != keywords.end()) {
        tokens.emplace(TYPE_KEYWORD, builder);
        return;
    }

    tokens.emplace(TYPE_IDENTIFIER, builder);
};



















constexpr lex::LookupTable num_table = {"0123456789", true};

void number_handler(StringIter& iter, Group& tokens) {
    const auto builder = iter.read_while([] (auto&, char c) {
        return num_table[c];
    });

    tokens.emplace(TYPE_LITERAL_FLOAT, builder);
};






























void string_handler(StringIter& iter, Group& tokens) {
    if (iter.match('"')) {
        tokens.emplace(TYPE_STRING);
        iter.incr();
        return;
    }

    iter.incr();


    const auto builder = iter.read_while([] (auto& iter, char c) {
        return not (c == '"' and iter.peek() != '\\');
    });


    iter.incr();
    tokens.emplace(TYPE_STRING, builder);
};





























void op_handler(StringIter& iter, Group& tokens) {
    switch (iter.peek()) {

        case '+':
            if (iter.match('=')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_PLUS_EQ, "+=");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_PLUS_EQ);
                });

                return;


            } else if (iter.match('+')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_INCR, "++");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_INCR);
                });

                return;


            } else {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_PLUS, "+");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_PLUS);
                });

                return;
            }



        case '<':
            if (iter.match('=')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_LESS_EQ, "<=");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_LESS_EQ);
                });

                return;


            } else {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_LESS, "<");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_LESS);
                });

                return;
            }



        case '>':
            if (iter.match('=')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MORE_EQ, ">=");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MORE_EQ);
                });

                return;


            } else {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MORE, ">");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MORE);
                });

                return;
            }



        case '-':
            if (iter.match('=')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MINUS_EQ, "-=");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MINUS_EQ);
                });

                return;


            } else if (iter.match('-')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_DECR, "--");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_DECR);
                });

                return;


            } else if (iter.match('>')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_ARROW, "->");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_ARROW);
                });

                return;


            } else {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MINUS, "-");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MINUS);
                });

                return;
            }



        case '*':
            if (iter.match('=')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MUL_EQ, "*=");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MUL_EQ);
                });

                return;


            } else {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MUL, "*");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_MUL);
                });

                return;
            }



        case '\'':
            ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                tokens.emplace(TYPE_OPERATOR_QUOTE, "'");
            });

            ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                tokens.emplace(TYPE_OPERATOR_QUOTE);
            });

            return;



        case '^':
            ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                tokens.emplace(TYPE_OPERATOR_RAISE, "^");
            });

            ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                tokens.emplace(TYPE_OPERATOR_RAISE);
            });

            return;



        case '|':
            if (iter.match('>')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_PIPE, "|>");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_PIPE);
                });

                return;
            }



        case '/':
            if (iter.match('/')) {
                iter.next_while([] (auto&, char c) {
                    return (c != '\n');
                });

                return;


            } else if (iter.match('=')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_DIV_EQ, "/=");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_DIV_EQ);
                });

                return;


            } else {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_DIV, "/");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_DIV);
                });

                return;
            }



        case '=':
            if (iter.match('=')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_COMPARE, "==");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_COMPARE);
                });

                return;

            } else {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_ASSIGN, "=");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_OPERATOR_ASSIGN);
                });

                return;
            }



        default:
            throw lex::exception::Error("unknown operator '" + std::string{iter.ptr()} + "'.");
    }

};



















void whitespace_handler(StringIter& iter, Group&) {
    iter.next_while([] (auto&, char c) {
        return (static_cast<bool>(std::isspace(c)));
    });
}








void left_brace_handler(StringIter&, Group& tokens) {
    ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_BRACE_LEFT, "{");
    });

    ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_BRACE_LEFT);
    });
}


void right_brace_handler(StringIter&, Group& tokens) {
    ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_BRACE_RIGHT, "}");
    });

    ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_BRACE_RIGHT);
    });
}






void left_bracket_handler(StringIter&, Group& tokens) {
    ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_BRACKET_LEFT, "[");
    });

    ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_BRACKET_LEFT);
    });
}

void right_bracket_handler(StringIter&, Group& tokens) {
    ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_BRACKET_RIGHT, "]");
    });

    ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_BRACKET_RIGHT);
    });
}







void left_paren_handler(StringIter&, Group& tokens) {
    ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_PAREN_LEFT, "(");
    });

    ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_PAREN_LEFT);
    });
}

void right_paren_handler(StringIter&, Group& tokens) {
    ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_PAREN_RIGHT, ")");
    });

    ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
        tokens.emplace(TYPE_PAREN_RIGHT);
    });
}






void seperator_handler(StringIter& iter, Group& tokens) {
    switch (iter.peek()) {
        case ':':
            if (iter.match(':')) {
                ws::module::run_if<lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_FUNC_DEF, "::");
                });

                ws::module::run_if<not lex::constant::PRINT_TOKENS>([&] {
                    tokens.emplace(TYPE_FUNC_DEF);
                });

                return;
            }
    }

    tokens.emplace(TYPE_SEPERATOR, std::string_view{iter.ptr(), 1});
}





void comment_handler(StringIter& iter, Group&) {
    iter.next_while([] (auto&, char c) {
        return (c != '\n');
    });
}