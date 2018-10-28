#pragma once


#include <string>
#include <alias.h>


namespace ws::lexer {
    struct StringIter {
        std::string buffer;
        std::string::size_type current = 0;


        StringIter(const std::string& buffer_): buffer(buffer_) {}



        // Iterate.
        char next() {
            current++;
            return buffer.at(current - 1);
        }

        char prev() {
            current--;
            return buffer.at(current + 1);
        }

        char next(const unsigned i) {
            current += i;
            return buffer.at(current - i);
        }

        char prev(const unsigned i) {
            current -= i;
            return buffer.at(current - i);
        }



        void incr() noexcept { current++; }
        void decr() noexcept { current--; }

        void incr(const unsigned i) noexcept { current += i; }
        void decr(const unsigned i) noexcept { current -= i; }









        // View characters.
        char at(const unsigned i)   const { return buffer.at(i);           }
        char peek(const unsigned i) const { return buffer.at(current + i); }
        char peek()             const { return buffer.at(current);     }






        // Info.
        std::string::size_type size() const noexcept { return buffer.size(); }

        bool is_end() const noexcept {
            return (current == size());
        }








        // Read N characters.
        const std::string read(const std::string::size_type& n) const {
            return buffer.substr(current + 1, n);
        }



        // Check if next character is expected and advance.
        bool match(char expect);


        // Skip over characters until a certain condition is met.
        void next_while(const alias::WhilePred& pred);


        const std::string read_while(const alias::WhilePred& pred);
    };
}