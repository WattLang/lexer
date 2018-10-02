#pragma once


#include <string>


namespace ws::lexer {
    struct Code {
        std::string str;
        int ptr = 0;


        Code(const std::string& str_): str(str_) {}


        char next()    { return str.at(ptr++); }
        char prev()    { return str.at(ptr--); }

        char current() const { return str.at(ptr);   }
        auto at(int i) const { return str.at(i); }

        void reset()    { ptr = 0; }
        void set(int i) { ptr = i; }


        int pos()  const { return ptr;        }
        int size() const { return str.size(); }


        // Are we at the final character?
        bool is_end() const {
            return (pos() >= (size() - 1));
        }


        std::string read_while(ReadIfPred pred) {
            std::string builder;

            while (true) {
                char c = current();
                builder += c;  // Append the character to the builder.


                if (is_end())
                    break;


                // If the next char is not valid, break and return builder.
                if (not pred(at(ptr + 1)))
                    break;


                next();
            }

            return builder;
        }
    };
}