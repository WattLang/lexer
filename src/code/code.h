#pragma once


#include <string>
#include <constants.h>
#include <aliases/aliases.h>


namespace ws::lexer {
    struct Code {
        std::string str;
        int ptr = 0;


        Code(const std::string& str_): str(str_) {}


        char next() { return str.at(ptr++); }
        char prev() { return str.at(ptr--); }

        char current()   const { return str.at(ptr);     }
        auto at(int i)   const { return str.at(i);       }
        auto peek(int i) const { return str.at(ptr + i); }

        void reset()    { ptr = 0; }
        void set(int i) { ptr = i; }


        int pos()  const { return ptr;        }
        int size() const { return str.size(); }


        // Are we at the final character?
        bool is_end() const {
            return (pos() >= (size() - 1));
        }


        std::string read_while(ws::lexer::ReadIfPred pred);
    };
}