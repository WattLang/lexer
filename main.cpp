#include <iostream>
#include <unordered_set>
#include <string>
#include <chrono>

#include <libs/module.h>

#include <constant.h>

#include <serializer/serializer.h>
#include <stringiter/stringiter.h>
#include <handlergroup/handlergroup.h>
#include <exception.h>

#include <lexer/lexer.h>










namespace lex = ws::lexer;

















enum: ws::token::type_t {
    TYPE_NULL,
    TYPE_IDENTIFIER,
    TYPE_STRING,
    TYPE_SEPERATOR,
    TYPE_COMMENT,

    TYPE_OPERATOR,

    TYPE_OPERATOR_PLUS_EQ,
    TYPE_OPERATOR_PLUS,

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
void ident_handler(lex::StringIter& iter, lex::Serializer& out) {
	// If we somehow inline this lambda, it will probably save quite a bit of time (atleast on bench.txt test)
    const auto builder = iter.read_while([] (lex::StringIter&, char c) {
		return ('A' >= c && c <= 'Z') || ('a' >= c && c <= 'z') || ('0' >= c && c <= '9') || c == '_';
    });

	// TODO: A way to serialize string_views directly without copying them to strings
	// out.print({TYPE_IDENTIFIER, 0, 0, std::string(builder)});
};



void number_handler(lex::StringIter& iter, lex::Serializer& out) {
    const auto builder = iter.read_while([] (lex::StringIter&, char c) {
		return ('0' >= c && c <= '9');
    });


	// TODO: A way to serialize string_views directly without copying them to strings
	// out.print({TYPE_LITERAL_FLOAT, 0, 0, std::string(builder) });
};



void string_handler(lex::StringIter& iter, lex::Serializer& out) {
    if (iter.match('"')) {
        out.print({TYPE_STRING, 0, 0, ""});
        iter.incr();
        return;
    }

    iter.incr();


    const auto builder = iter.read_while([] (lex::StringIter& iter, char c) {
        return not (c == '"' and iter.peek() != '\\');
    });


    iter.incr();
	// TODO: A way to serialize string_views directly without copying them to strings
	// out.print({TYPE_STRING, 0, 0, std::string(builder) });
};




void op_handler(lex::StringIter& iter, lex::Serializer& out) {
    switch (iter.peek()) {

        case '+':
            iter.match('=') ?
                out.print({TYPE_OPERATOR_PLUS_EQ, 0, 0, "+="}) :
                out.print({TYPE_OPERATOR_PLUS, 0, 0, "+"});

            break;


        case '-':
            iter.match('=') ?
                out.print({TYPE_OPERATOR_MINUS_EQ, 0, 0, "-="}) :
                out.print({TYPE_OPERATOR_MINUS, 0, 0, "-"});

            break;


        case '*':
            iter.match('=') ?
                out.print({TYPE_OPERATOR_MUL_EQ, 0, 0, "*="}) :
                out.print({TYPE_OPERATOR_MUL, 0, 0, "*"});

            break;


        case '|':
            if (iter.match('>')) {
                out.print({TYPE_OPERATOR_PIPE, 0, 0, "|>"});
            }

            break;


        case '/':
            if (iter.match('/'))
                iter.next_while([] (lex::StringIter&, char c) {
                    return (c != '\n');
                });

            else if (iter.match('='))
                out.print({TYPE_OPERATOR_DIV_EQ, 0, 0, "/="});

            else
                out.print({TYPE_OPERATOR_DIV, 0, 0, "/"});

            break;


        case '=':
            iter.match('=') ?
                out.print({TYPE_OPERATOR_COMPARE, 0, 0, "=="}) :
                out.print({TYPE_OPERATOR_ASSIGN, 0, 0, "="});

            break;



        default:
            throw lex::exception::LexicalError("unknown operator '" + std::string{iter.peek()} + "'.");
    }

};



























int main(int, char const* []) {
    const auto start = std::chrono::steady_clock::now();




	auto data = ws::module::receive_all(lex::constant::INPUT_BUFFER_SIZE);
	lex::StringIter iter { std::string_view(data) };

    lex::Serializer out;





    constexpr lex::HandlerGroup handlers = {
        {
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_",
            ident_handler
        },


        {"\n\t\r ", [](lex::StringIter&, lex::Serializer&){  }},
        {"0123456789", number_handler},
        {"|+-*/=", op_handler},


        {
            "{",
            [](lex::StringIter&, lex::Serializer& out){
                out.print({TYPE_BRACE_LEFT, 0, 0, "{"});
            }
        },


        {
            "}",
            [](lex::StringIter&, lex::Serializer& out){
                out.print({TYPE_BRACE_RIGHT, 0, 0, "}"});
            }
        },


        {
            "[",
            [](lex::StringIter&, lex::Serializer& out){
                out.print({TYPE_BRACKET_LEFT, 0, 0, "["});
            }
        },


        {
            "]",
            [](lex::StringIter&, lex::Serializer& out){
                out.print({TYPE_BRACKET_RIGHT, 0, 0, "]"});
            }
        },


        {
            "(",
            [](lex::StringIter&, lex::Serializer& out){
                out.print({TYPE_PAREN_LEFT, 0, 0, "("});
            }
        },


        {
            ")",
            [](lex::StringIter&, lex::Serializer& out){
                out.print({TYPE_PAREN_RIGHT, 0, 0, ")"});
            }
        },


        {
            ",:;",
            [](lex::StringIter& iter, lex::Serializer& out){
                out.print({TYPE_SEPERATOR, 0, 0, std::string{iter.peek()}});
            }
        },


        {"\"", string_handler},

        {"#", [](lex::StringIter& iter, lex::Serializer&){
                iter.next_while([] (lex::StringIter&, char c) {
                    return (c != '\n');
                });


            }
        }
    };




    int ret = lex::lexer(iter, out, handlers);





    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    ws::module::successln(ws::module::style::bold, "Finished in ", duration, "ms!");


    return ret;
}