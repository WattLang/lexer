#pragma once

using Group      = lex::Group;
using StringIter = lex::StringIter;

lex::Position CURRENT_POSITION;




enum: ws::token::Type {
    TYPE_NULL,
    TYPE_IDENTIFIER,
    TYPE_KEYWORD,
    TYPE_STRING,
    TYPE_SEPERATOR,
    TYPE_COMMA,
    TYPE_DOT,
    TYPE_COMMENT,
    TYPE_DOUBLE_COLON,
    TYPE_SINGLE_COLON,
    TYPE_SEMICOLON,
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
    TYPE_WHITESPACE,
    TYPE_KEYWORD_IF,
    TYPE_KEYWORD_ELSE,
    TYPE_KEYWORD_WHILE,
    TYPE_KEYWORD_FOR,
    TYPE_KEYWORD_VAR,
    TYPE_KEYWORD_LET,
    TYPE_KEYWORD_RETURN,
    TYPE_KEYWORD_BREAK,
    TYPE_KEYWORD_CONTINUE,
    TYPE_KEYWORD_TRUE,
    TYPE_KEYWORD_FALSE,
    TYPE_KEYWORD_STRUCT,
    TYPE_KEYWORD_NIL,
    TYPE_KEYWORD_SELF
};


constexpr const char* token_strings[] = {
    "TYPE_NULL",
    "TYPE_IDENTIFIER",
    "TYPE_KEYWORD",
    "TYPE_STRING",
    "TYPE_SEPERATOR",
    "TYPE_COMMA",
    "TYPE_DOT",
    "TYPE_COMMENT",
    "TYPE_DOUBLE_COLON",
    "TYPE_SINGLE_COLON",
    "TYPE_SEMICOLON",
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
    "TYPE_WHITESPACE",
    "TYPE_KEYWORD_IF",
    "TYPE_KEYWORD_ELSE",
    "TYPE_KEYWORD_WHILE",
    "TYPE_KEYWORD_FOR",
    "TYPE_KEYWORD_VAR",
    "TYPE_KEYWORD_LET",
    "TYPE_KEYWORD_RETURN",
    "TYPE_KEYWORD_BREAK",
    "TYPE_KEYWORD_CONTINUE",
    "TYPE_KEYWORD_TRUE",
    "TYPE_KEYWORD_FALSE",
    "TYPE_KEYWORD_STRUCT",
    "TYPE_KEYWORD_NIL",
    "TYPE_KEYWORD_SELF"
};



















#define STATELESS_HANDLER(name, type, str) \
    void name(StringIter& iter, Group& tokens) { \
        const auto old_pos = CURRENT_POSITION; \
     \
        if constexpr(lex::constant::PRINT_TOKENS) { \
            tokens.emplace(type, old_pos, str); \
     \
        } else { \
            tokens.emplace(type, old_pos); \
        } \
     \
        CURRENT_POSITION.next(); \
    }






void on_newline(StringIter& iter, Group& tokens) {
    CURRENT_POSITION.nextln();
}


void on_whitespace(StringIter& iter, Group& tokens) {
    CURRENT_POSITION.next();
}



STATELESS_HANDLER(on_left_paren,    TYPE_PAREN_LEFT, "(")
STATELESS_HANDLER(on_right_paren,   TYPE_PAREN_LEFT, ")")

STATELESS_HANDLER(on_left_brace,    TYPE_PAREN_LEFT, "[")
STATELESS_HANDLER(on_right_brace,   TYPE_PAREN_LEFT, "]")

STATELESS_HANDLER(on_left_bracket,  TYPE_PAREN_LEFT, "{")
STATELESS_HANDLER(on_right_bracket, TYPE_PAREN_LEFT, "}")


STATELESS_HANDLER(on_comma,       TYPE_COMMA,     ",")
STATELESS_HANDLER(on_dot,         TYPE_DOT,       ".")
STATELESS_HANDLER(on_semicolon,   TYPE_SEMICOLON, ";")
























constexpr lex::LookupTable ident_table{
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
};

constexpr std::pair<std::string_view, ws::token::Type> keyw[] = {
    {"var",      TYPE_KEYWORD_VAR},
    {"struct",   TYPE_KEYWORD_STRUCT},
    {"self",     TYPE_KEYWORD_SELF},
    {"if",       TYPE_KEYWORD_IF},
    {"else",     TYPE_KEYWORD_ELSE},
    {"for",      TYPE_KEYWORD_FOR},
    {"while",    TYPE_KEYWORD_WHILE},
    {"break",    TYPE_KEYWORD_BREAK},
    {"continue", TYPE_KEYWORD_CONTINUE},
    {"return",   TYPE_KEYWORD_RETURN},
    {"nil",      TYPE_KEYWORD_NIL},
    {"true",     TYPE_KEYWORD_TRUE},
    {"false",    TYPE_KEYWORD_FALSE}
};


void on_ident(StringIter& iter, Group& tokens) {
    const auto old_pos = CURRENT_POSITION;

    const auto builder = iter.read_while([] (auto&, char c) {
        CURRENT_POSITION.next();
        return ident_table[c];
    });

    CURRENT_POSITION.next(builder.size());

    for (auto it = std::begin(keyw); it != std::end(keyw); ++it) {
        if (it->first == builder) {
            const auto& [str, type] = *it;

            if constexpr(lex::constant::PRINT_TOKENS)
                tokens.emplace(type, old_pos, builder);

            else
                tokens.emplace(type, old_pos);

            return;
        }
    }

    tokens.emplace(TYPE_IDENTIFIER, old_pos, builder);
};



























/*constexpr lex::LookupTable op_table{"+-*=<>/"};

constexpr std::pair<std::string_view, ws::token::Type> op_keyw[] = {
    {"+",  TYPE_OPERATOR_PLUS},
    {"-",  TYPE_OPERATOR_MINUS},
    {"/",  TYPE_OPERATOR_DIV},
    {"*",  TYPE_OPERATOR_MUL},
    {"++", TYPE_OPERATOR_INCR},
    {"--", TYPE_OPERATOR_DECR},
    {"+=", TYPE_OPERATOR_PLUS_EQ},
    {"-=", TYPE_OPERATOR_MINUS_EQ},
    {"/=", TYPE_OPERATOR_DIV_EQ},
    {"*=", TYPE_OPERATOR_MUL_EQ},
    {"<",  TYPE_OPERATOR_LESS},
    {">",  TYPE_OPERATOR_MORE},
    {"=",  TYPE_OPERATOR_ASSIGN},
    {"<=", TYPE_OPERATOR_LESS_EQ},
    {">=", TYPE_OPERATOR_MORE_EQ},
    {"==", TYPE_OPERATOR_COMPARE},
    {"->", TYPE_OPERATOR_ARROW},
    {"//", TYPE_COMMENT}
};

void on_op(StringIter& iter, Group& tokens) {
    const auto old_pos = CURRENT_POSITION;

    const auto op = iter.read_while([] (auto&, char c) {
        CURRENT_POSITION.next();
        return op_table[c];
    });

    CURRENT_POSITION.next(op.size());

    for (auto it = std::begin(op_keyw); it != std::end(op_keyw); ++it) {
        if (it->first == op) {
            const auto& [str, type] = *it;

            if constexpr(lex::constant::PRINT_TOKENS)
                tokens.emplace(type, old_pos, op);

            else
                tokens.emplace(type, old_pos);

            return;
        }
    }

    throw lex::exception::Error(
        "unknown operator '" + std::string{op} + "'."
    );
};*/



















#define HANDLE_OP(type, str, inc) \
    if constexpr(lex::constant::PRINT_TOKENS) { \
        tokens.emplace(type, old_pos, str); \
    } \
 \
    if constexpr(not lex::constant::PRINT_TOKENS) { \
        tokens.emplace(type, old_pos); \
    } \
 \
    CURRENT_POSITION.next(inc); \
 \
    return;





void on_op(StringIter& iter, Group& tokens) {
    const auto old_pos = CURRENT_POSITION;

    switch (iter.peek()) {

        case '+':
            if (iter.match('=')) {
                HANDLE_OP(TYPE_OPERATOR_PLUS_EQ, "+=", 2)

            } else if (iter.match('+')) {
                HANDLE_OP(TYPE_OPERATOR_INCR, "++", 2)

            } else {
                HANDLE_OP(TYPE_OPERATOR_PLUS, "+", 1)
            }


        case '<':
            if (iter.match('=')) {
                HANDLE_OP(TYPE_OPERATOR_LESS_EQ, "<=", 2)

            } else {
                HANDLE_OP(TYPE_OPERATOR_LESS, "<", 1)
            }


        case '>':
            if (iter.match('=')) {
                HANDLE_OP(TYPE_OPERATOR_MORE_EQ, ">=", 2)

            } else {
                HANDLE_OP(TYPE_OPERATOR_MORE, ">", 1)
            }


        case '-':
            if (iter.match('=')) {
                HANDLE_OP(TYPE_OPERATOR_MINUS_EQ, "-=", 2)

            } else if (iter.match('-')) {
                HANDLE_OP(TYPE_OPERATOR_DECR, "--", 2)

            } else if (iter.match('>')) {
                HANDLE_OP(TYPE_OPERATOR_ARROW, "->", 2)

            } else {
                HANDLE_OP(TYPE_OPERATOR_MINUS, "-", 1)
            }


        case '*':
            if (iter.match('=')) {
                HANDLE_OP(TYPE_OPERATOR_MUL_EQ, "*=", 2)

            } else {
                HANDLE_OP(TYPE_OPERATOR_MUL, "*", 1)
            }


        case '=':
            if (iter.match('=')) {
                HANDLE_OP(TYPE_OPERATOR_COMPARE, "==", 2)

            } else {
                HANDLE_OP(TYPE_OPERATOR_ASSIGN, "=", 1)
            }


         case '/':
            if (iter.match('/')) {
                iter.next_while([] (auto& iter2, char c) {
                    CURRENT_POSITION.next();
                    return (c != '\n');
                });

                return;

            } else if (iter.match('=')) {
                HANDLE_OP(TYPE_OPERATOR_DIV_EQ, "/=", 2)

            } else {
                HANDLE_OP(TYPE_OPERATOR_DIV, "/", 1)
            }


        default: break;
    }


    throw lex::exception::Error(
        "unknown operator '" + std::string{iter.ptr(), 1} + "'."
    );
};

















constexpr lex::LookupTable num_table{"0123456789"};

void on_number(StringIter& iter, Group& tokens) {
    const auto old_pos = CURRENT_POSITION;

    const auto builder = iter.read_while([] (auto&, char c) {
        CURRENT_POSITION.next();
        return num_table[c];
    });

    tokens.emplace(TYPE_LITERAL_FLOAT, old_pos, builder);
};






























void on_string(StringIter& iter, Group& tokens) {
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











void on_colon(StringIter& iter, Group& tokens) {
    const auto old_pos = CURRENT_POSITION;

    if constexpr(lex::constant::PRINT_TOKENS) {
        if (iter.match(':')) {
            CURRENT_POSITION.next(2);
            tokens.emplace(TYPE_DOUBLE_COLON, old_pos, "::");

        } else {
            tokens.emplace(TYPE_SINGLE_COLON, old_pos, ":");
            CURRENT_POSITION.next();
        }


    } else {
        if (iter.match(':')) {
            tokens.emplace(TYPE_DOUBLE_COLON, old_pos);
            CURRENT_POSITION.next(2);

        } else {
            tokens.emplace(TYPE_SINGLE_COLON, old_pos);
            CURRENT_POSITION.next();
        }
    }
}




void on_comment(StringIter& iter, Group&) {
    iter.next_while([] (auto&, char c) {
        CURRENT_POSITION.next();
        return (c != '\n');
    });
}