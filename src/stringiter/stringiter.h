#pragma once


#include <string_view>
#include <alias.h>
#include <libs/module.h>


namespace ws::lexer {
    struct StringIter {
        using ptr_type = const char*;
        using buffer_type = std::string_view;


        // Data.
        buffer_type buffer;
        ptr_type current = nullptr;



        // Constructor.
        StringIter(buffer_type buffer_):
            buffer(buffer_), current(buffer.data())
        {

        }


        // Getters.
        ptr_type ptr() const { return current; }


        // Iterate.
        char next()          { return *current++; }
        void incr() noexcept { current++;         }


        // View characters.
        char peek(int i) const { return *(current + i); }
        char peek()            const { return *(current);     }


        // Info.
        buffer_type::size_type size() const noexcept {
            return buffer.size();
        }

        bool is_end(int offset = 0) const noexcept {
            return current >= ((buffer.data() + size()) + offset);
        }


        // Conditional consume.
        bool match(char expect);

        // Skip characters until predicate fails.
        void next_while(WhilePred pred);

        // Consume characters while predicate is satisfied.
        buffer_type read_while(WhilePred pred);
    };
}