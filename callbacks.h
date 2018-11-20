#pragma once






using Group      = lex::alias::Group;
using StringIter = lex::StringIter;




lex::alias::Position CURRENT_POSITION;




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
    TYPE_LITERAL_FLOAT,
    TYPE_WHITESPACE
};


constexpr const char* token_strings[] = {
    "TYPE_NULL",
    "TYPE_IDENTIFIER",
    "TYPE_KEYWORD",
    "TYPE_STRING",
    "TYPE_SEPERATOR",
    "TYPE_COMMENT",
    "TYPE_FUNC_DEF",
    "TYPE_OPERATOR",
    "TYPE_OPERATOR_RAISE",
    "TYPE_OPERATOR_ARROW",
    "TYPE_OPERATOR_QUOTE",
    "TYPE_OPERATOR_INCR",
    "TYPE_OPERATOR_DECR",
    "TYPE_OPERATOR_PLUS_EQ",
    "TYPE_OPERATOR_PLUS",
    "TYPE_OPERATOR_LESS_EQ",
    "TYPE_OPERATOR_LESS",
    "TYPE_OPERATOR_MORE_EQ",
    "TYPE_OPERATOR_MORE",
    "TYPE_OPERATOR_MINUS_EQ",
    "TYPE_OPERATOR_MINUS",
    "TYPE_OPERATOR_DIV_EQ",
    "TYPE_OPERATOR_DIV",
    "TYPE_OPERATOR_MUL_EQ",
    "TYPE_OPERATOR_MUL",
    "TYPE_OPERATOR_ASSIGN",
    "TYPE_OPERATOR_COMPARE",
    "TYPE_OPERATOR_PIPE",
    "TYPE_PAREN_LEFT",
    "TYPE_PAREN_RIGHT",
    "TYPE_BRACKET_LEFT",
    "TYPE_BRACKET_RIGHT",
    "TYPE_BRACE_LEFT",
    "TYPE_BRACE_RIGHT",
    "TYPE_LITERAL_FLOAT",
    "TYPE_WHITESPACE"
};
























constexpr lex::LookupTable ident_table = {
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", true
};

const std::unordered_set<std::string_view> keywords = {
    "var", "struct", "self",
    "if", "else", "for", "while",
    "break", "continue", "return",
    "nil", "true", "false"
};

void ident_handler(StringIter& iter, Group& tokens) {
    const auto old_pos = CURRENT_POSITION;

    const auto builder = iter.read_while([] (auto& iter2, char c) {
        CURRENT_POSITION.next();
        return ident_table[c];
    });


    if (keywords.count(builder)) {
        tokens.emplace(TYPE_KEYWORD, old_pos, builder);
        return;
    }


    tokens.emplace(TYPE_IDENTIFIER, old_pos, builder);
};



















constexpr lex::LookupTable num_table = {"0123456789", true};

void number_handler(StringIter& iter, Group& tokens) {
    const auto old_pos = CURRENT_POSITION;

    const auto builder = iter.read_while([] (auto& iter2, char c) {
        CURRENT_POSITION.next();
        return num_table[c];
    });

    tokens.emplace(TYPE_LITERAL_FLOAT, old_pos, builder);
};






























void string_handler(StringIter& iter, Group& tokens) {
    if (iter.match('"')) {
        tokens.emplace(TYPE_STRING, CURRENT_POSITION);
        iter.incr();
        CURRENT_POSITION.next(2);
        return;
    }

    const auto old_pos = CURRENT_POSITION;

    iter.incr();


    const auto builder = iter.read_while([] (auto& iter2, char c) {
        CURRENT_POSITION.next();
        return not (c == '"' and iter2.peek() != '\\');
    });


    iter.incr();
    CURRENT_POSITION.next(2);
    tokens.emplace(TYPE_STRING, old_pos, builder);
};





























void op_handler(StringIter& iter, Group& tokens) {
    const auto old_pos = CURRENT_POSITION;

    switch (iter.peek()) {

        case '+':
            if (iter.match('=')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_PLUS_EQ, old_pos, "+=");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_PLUS_EQ, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;


            } else if (iter.match('+')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_INCR, old_pos, "++");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_INCR, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;


            } else {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_PLUS, old_pos, "+");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_PLUS, old_pos);
                }

                CURRENT_POSITION.next();

                return;
            }



        case '<':
            if (iter.match('=')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_LESS_EQ, old_pos, "<=");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_LESS_EQ, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;


            } else {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_LESS, old_pos, "<");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_LESS, old_pos);
                }

                CURRENT_POSITION.next();

                return;
            }



        case '>':
            if (iter.match('=')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MORE_EQ, old_pos, ">=");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MORE_EQ, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;


            } else {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MORE, old_pos, ">");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MORE, old_pos);
                }

                CURRENT_POSITION.next();

                return;
            }



        case '-':
            if (iter.match('=')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MINUS_EQ, old_pos, "-=");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MINUS_EQ, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;


            } else if (iter.match('-')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_DECR, old_pos, "--");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_DECR, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;


            } else if (iter.match('>')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_ARROW, old_pos, "->");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_ARROW, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;


            } else {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MINUS, old_pos, "-");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MINUS, old_pos);
                }

                CURRENT_POSITION.next();

                return;
            }



        case '*':
            if (iter.match('=')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MUL_EQ, old_pos, "*=");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MUL_EQ, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;


            } else {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MUL, old_pos, "*");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_MUL, old_pos);
                }

                CURRENT_POSITION.next();

                return;
            }



        case '\'':
            if constexpr(lex::constant::PRINT_TOKENS) {
                tokens.emplace(TYPE_OPERATOR_QUOTE, old_pos, "'");
            }

            if constexpr(not lex::constant::PRINT_TOKENS) {
                tokens.emplace(TYPE_OPERATOR_QUOTE, old_pos);
            }

            CURRENT_POSITION.next();

            return;



        case '^':
            if constexpr(lex::constant::PRINT_TOKENS) {
                tokens.emplace(TYPE_OPERATOR_RAISE, old_pos, "^");
            }

            if constexpr(not lex::constant::PRINT_TOKENS) {
                tokens.emplace(TYPE_OPERATOR_RAISE, old_pos);
            }

            CURRENT_POSITION.next();

            return;



        case '|':
            if (iter.match('>')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_PIPE, old_pos, "|>");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_PIPE, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;
            }

            break;


        case '/':
            if (iter.match('/')) {
                iter.next_while([] (auto& iter2, char c) {
                    CURRENT_POSITION.next();
                    return (c != '\n');
                });

                return;


            } else if (iter.match('=')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_DIV_EQ, old_pos, "/=");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_DIV_EQ, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;


            } else {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_DIV, old_pos, "/");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_DIV, old_pos);
                }

                CURRENT_POSITION.next();

                return;
            }



        case '=':
            if (iter.match('=')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_COMPARE, old_pos, "==");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_COMPARE, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;

            } else {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_ASSIGN, old_pos, "=");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_OPERATOR_ASSIGN, old_pos);
                }

                CURRENT_POSITION.next();

                return;
            }



        default:
            throw lex::exception::Error("unknown operator '" + std::string{iter.ptr()} + "'.");
    }

};



















void whitespace_handler(StringIter& iter, Group& tokens) {
    if constexpr(lex::constant::PRINT_TOKENS) {
        const auto builder = iter.read_while([] (auto& iter2, char c) {
            if (iter2.peek() == '\n')
                CURRENT_POSITION.nextln();

            else
                CURRENT_POSITION.next();

            return (static_cast<bool>(std::isspace(c)));
        });

        //tokens.emplace(TYPE_WHITESPACE, CURRENT_POSITION, builder);
    }



    if constexpr(not lex::constant::PRINT_TOKENS) {
        iter.next_while([] (auto& iter2, char c) {
            if (iter2.peek() == '\n')
                CURRENT_POSITION.nextln();

            else
                CURRENT_POSITION.next();

            return (static_cast<bool>(std::isspace(c)));
        });
    }
}








void left_brace_handler(StringIter& iter, Group& tokens) {
    if constexpr(lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_BRACE_LEFT, CURRENT_POSITION, "{");
    }

    if constexpr(not lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_BRACE_LEFT, CURRENT_POSITION);
    }

    CURRENT_POSITION.next();
}


void right_brace_handler(StringIter& iter, Group& tokens) {
    if constexpr(lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_BRACE_RIGHT, CURRENT_POSITION, "}");
    }

    if constexpr(not lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_BRACE_RIGHT, CURRENT_POSITION);
    }

    CURRENT_POSITION.next();
}






void left_bracket_handler(StringIter& iter, Group& tokens) {
    if constexpr(lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_BRACKET_LEFT, CURRENT_POSITION, "[");
    }

    if constexpr(not lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_BRACKET_LEFT, CURRENT_POSITION);
    }

    CURRENT_POSITION.next();
}

void right_bracket_handler(StringIter& iter, Group& tokens) {
    if constexpr(lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_BRACKET_RIGHT, CURRENT_POSITION, "]");
    }

    if constexpr(not lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_BRACKET_RIGHT, CURRENT_POSITION);
    }

    CURRENT_POSITION.next();
}







void left_paren_handler(StringIter& iter, Group& tokens) {
    if constexpr(lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_PAREN_LEFT, CURRENT_POSITION, "(");
    }

    if constexpr(not lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_PAREN_LEFT, CURRENT_POSITION);
    }

    CURRENT_POSITION.next();
}

void right_paren_handler(StringIter& iter, Group& tokens) {
    if constexpr(lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_PAREN_RIGHT, CURRENT_POSITION, ")");
    }

    if constexpr(not lex::constant::PRINT_TOKENS) {
        tokens.emplace(TYPE_PAREN_RIGHT, CURRENT_POSITION);
    }

    CURRENT_POSITION.next();
}






void seperator_handler(StringIter& iter, Group& tokens) {
    const auto old_pos = CURRENT_POSITION;

    switch (iter.peek()) {
        case ':':
            if (iter.match(':')) {
                if constexpr(lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_FUNC_DEF, old_pos, "::");
                }

                if constexpr(not lex::constant::PRINT_TOKENS) {
                    tokens.emplace(TYPE_FUNC_DEF, old_pos);
                }

                CURRENT_POSITION.next(2);

                return;
            }

        default:
            break;
    }

    CURRENT_POSITION.next();

    tokens.emplace(TYPE_SEPERATOR, old_pos, std::string_view{iter.ptr(), 1});
}





void comment_handler(StringIter& iter, Group&) {
    iter.next_while([] (auto& iter2, char c) {
        CURRENT_POSITION.next();
        return (c != '\n');
    });
}