#pragma once




enum: ws::token::type_t {
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

    TYPE_LITERAL_FLOAT,
};




















// Handlers.
void ident_handler(lex::StringIter& iter, lex::alias::TokenGroup& tokens) {
    const auto builder = iter.read_while([] (lex::StringIter&, char c) {
        auto ch = static_cast<unsigned char>(c);
        return (std::isalpha(c) || std::isdigit(c) || c == '_');
    });

    tokens.emplace_back(TYPE_IDENTIFIER, std::move(builder));
};





















void number_handler(lex::StringIter& iter, lex::alias::TokenGroup& tokens) {
    const auto builder = iter.read_while([] (lex::StringIter&, char c) {
        auto ch = static_cast<unsigned char>(c);
        return (static_cast<bool>(std::isdigit(c)));
    });

    tokens.emplace_back(TYPE_LITERAL_FLOAT, std::move(builder));
};






























void string_handler(lex::StringIter& iter, lex::alias::TokenGroup& tokens) {
    if (iter.match('"')) {
        tokens.emplace_back(TYPE_STRING);
        iter.incr();
        return;
    }

    iter.incr();


    const auto builder = iter.read_while([] (lex::StringIter& iter, char c) {
        return not (c == '"' and iter.peek() != '\\');
    });


    iter.incr();
    tokens.emplace_back(TYPE_STRING, std::move(builder));
};





























void op_handler(lex::StringIter& iter, lex::alias::TokenGroup& tokens) {
    switch (iter.peek()) {

        case '+':
            iter.match('=') ?
                tokens.emplace_back(TYPE_OPERATOR_PLUS_EQ) :
                tokens.emplace_back(TYPE_OPERATOR_PLUS);

            return;


        case '<':
            iter.match('=') ?
                tokens.emplace_back(TYPE_OPERATOR_LESS_EQ) :
                tokens.emplace_back(TYPE_OPERATOR_LESS);

            return;


        case '>':
            iter.match('=') ?
                tokens.emplace_back(TYPE_OPERATOR_MORE_EQ) :
                tokens.emplace_back(TYPE_OPERATOR_MORE);

            return;


        case '-':
            iter.match('=') ?
                tokens.emplace_back(TYPE_OPERATOR_MINUS_EQ) :
                tokens.emplace_back(TYPE_OPERATOR_MINUS);

            return;


        case '*':
            iter.match('=') ?
                tokens.emplace_back(TYPE_OPERATOR_MUL_EQ) :
                tokens.emplace_back(TYPE_OPERATOR_MUL);

            return;


        case '\'':
            tokens.emplace_back(TYPE_OPERATOR_QUOTE);

            return;


        case '^':
            tokens.emplace_back(TYPE_OPERATOR_RAISE);

            return;


        case '|':
            if (iter.match('>'))
                tokens.emplace_back(TYPE_OPERATOR_PIPE);

            return;


        case '/':
            if (iter.match('/'))
                iter.next_while([] (lex::StringIter&, char c) {
                    return (c != '\n');
                });

            else if (iter.match('='))
                tokens.emplace_back(TYPE_OPERATOR_DIV_EQ);

            else
                tokens.emplace_back(TYPE_OPERATOR_DIV);

            return;


        case '=':
            iter.match('=') ?
                tokens.emplace_back(TYPE_OPERATOR_COMPARE) :
                tokens.emplace_back(TYPE_OPERATOR_ASSIGN);

            return;



        default:
            throw lex::exception::Error("unknown operator '" + std::string{iter.peek()} + "'.");
    }

};



















void whitespace_handler(lex::StringIter& iter, lex::alias::TokenGroup&) {
    iter.next_while([] (lex::StringIter&, char c) {
        return (static_cast<bool>(std::isspace(c)));
    });
}








void left_brace_handler(lex::StringIter&, lex::alias::TokenGroup& tokens) {
    tokens.emplace_back(TYPE_BRACE_LEFT);
}


void right_brace_handler(lex::StringIter&, lex::alias::TokenGroup& tokens) {
    tokens.emplace_back(TYPE_BRACE_RIGHT);
}






void left_bracket_handler(lex::StringIter&, lex::alias::TokenGroup& tokens) {
    tokens.emplace_back(TYPE_BRACKET_LEFT);
}

void right_bracket_handler(lex::StringIter&, lex::alias::TokenGroup& tokens) {
    tokens.emplace_back(TYPE_BRACKET_RIGHT);
}







void left_paren_handler(lex::StringIter&, lex::alias::TokenGroup& tokens) {
    tokens.emplace_back(TYPE_PAREN_LEFT);
}

void right_paren_handler(lex::StringIter&, lex::alias::TokenGroup& tokens) {
    tokens.emplace_back(TYPE_PAREN_RIGHT);
}






void seperator_handler(lex::StringIter& iter, lex::alias::TokenGroup& tokens) {
    tokens.emplace_back(TYPE_SEPERATOR, std::string{iter.peek()});
}





void comment_handler(lex::StringIter& iter, lex::alias::TokenGroup&) {
    iter.next_while([] (lex::StringIter&, char c) {
        return (c != '\n');
    });
}