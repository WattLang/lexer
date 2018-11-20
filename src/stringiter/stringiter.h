#pragma once


#include <string_view>
#include <alias.h>
#include <libs/module.h>


namespace ws::lexer {
    struct StringIter {
        std::string_view buffer;
        const char *current = nullptr;



        StringIter(std::string_view buffer_):
            buffer(buffer_),
            current(buffer.data())
        {}




        const char* ptr() const {
            return current;
        }





        // Iterate.
        char next() {
            return *current++;
        }

        void incr() noexcept { current++; }



        // View characters.
        char peek(const unsigned i) const { return *(current + i); }
        char peek()                 const { return *(current);     }



        // Info.
        std::string_view::size_type size() const noexcept {
            return buffer.size();
        }

        bool is_end(int offset = 0) const noexcept {
            return current >= ((buffer.data() + size()) + offset);
        }




        // Check if next character is expected and advance.
        bool match(char expect);

        // Skip over characters until a certain condition is met.
        void next_while(alias::WhilePred pred);
        std::string_view read_while(alias::WhilePred pred);
    };
}