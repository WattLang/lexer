#pragma once



using Group = lex::alias::Group;
using StringIter = lex::StringIter;




enum: ws::token::Type {
    TYPE_NULL,
    TYPE_IDENTIFIER,
    TYPE_STRING,
    TYPE_SEPERATOR,
    TYPE_COMMENT,

    TYPE_OPERATOR,

    TYPE_OPERATOR_RAISE,

    TYPE_OPERATOR_QUOTE,

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

void ident_handler(StringIter& iter, Group& tokens) {
    const auto builder = iter.read_while([] (auto&, char c) {
        return ident_table[c];
    });


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
                tokens.emplace(TYPE_OPERATOR_PLUS_EQ);

                return;


            } else {
                tokens.emplace(TYPE_OPERATOR_PLUS);

                return;
            }



        case '<':
            if (iter.match('=')) {
                tokens.emplace(TYPE_OPERATOR_LESS_EQ);

                return;


            } else {
                tokens.emplace(TYPE_OPERATOR_LESS);

                return;
            }



        case '>':
            if (iter.match('=')) {
                tokens.emplace(TYPE_OPERATOR_MORE_EQ);

                return;


            } else {
                tokens.emplace(TYPE_OPERATOR_MORE);

                return;
            }



        case '-':
            if (iter.match('=')) {
                tokens.emplace(TYPE_OPERATOR_MINUS_EQ);

                return;


            } else {
                tokens.emplace(TYPE_OPERATOR_MINUS);

                return;
            }



        case '*':
            if (iter.match('=')) {
                tokens.emplace(TYPE_OPERATOR_MUL_EQ);

                return;


            } else {
                tokens.emplace(TYPE_OPERATOR_MUL);

                return;
            }



        case '\'':
            tokens.emplace(TYPE_OPERATOR_QUOTE);

            return;



        case '^':
            tokens.emplace(TYPE_OPERATOR_RAISE);

            return;



        case '|':
            if (iter.match('>')) {
                tokens.emplace(TYPE_OPERATOR_PIPE);

                return;
            }



        case '/':
            if (iter.match('/')) {
                iter.next_while([] (auto&, char c) {
                    return (c != '\n');
                });

                return;


            } else if (iter.match('=')) {
                tokens.emplace(TYPE_OPERATOR_DIV_EQ);

                return;


            } else {
                tokens.emplace(TYPE_OPERATOR_DIV);

                return;
            }



        case '=':
            if (iter.match('=')) {
                tokens.emplace(TYPE_OPERATOR_COMPARE);

                return;


            } else {
                tokens.emplace(TYPE_OPERATOR_ASSIGN);

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
    tokens.emplace(TYPE_BRACE_LEFT);
}


void right_brace_handler(StringIter&, Group& tokens) {
    tokens.emplace(TYPE_BRACE_RIGHT);
}






void left_bracket_handler(StringIter&, Group& tokens) {
    tokens.emplace(TYPE_BRACKET_LEFT);
}

void right_bracket_handler(StringIter&, Group& tokens) {
    tokens.emplace(TYPE_BRACKET_RIGHT);
}







void left_paren_handler(StringIter&, Group& tokens) {
    tokens.emplace(TYPE_PAREN_LEFT);
}

void right_paren_handler(StringIter&, Group& tokens) {
    tokens.emplace(TYPE_PAREN_RIGHT);
}






void seperator_handler(StringIter& iter, Group& tokens) {
    tokens.emplace(TYPE_SEPERATOR, std::string_view{iter.ptr(), 1});
}





void comment_handler(StringIter& iter, Group&) {
    iter.next_while([] (auto&, char c) {
        return (c != '\n');
    });
}