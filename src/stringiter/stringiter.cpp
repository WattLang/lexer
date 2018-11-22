#include "stringiter.h"


#include <constant.h>


namespace ws::lexer {
    bool StringIter::match(char expect) {
        auto b = (peek(1) == expect);

        // If we have a match, increment the current ptr.
        if (b) incr();

        return b;
    }


    void StringIter::next_while(WhilePred pred) {
        // Skip characters until predicate fails.
        while (pred(*this, peek(1)))
            incr();
    }


    std::string_view StringIter::read_while(WhilePred pred) {
        const char* begin = current;
        std::string::size_type length = 1;


        // Consume characters until predicate fails.
        for (; pred(*this, peek(1)); ++length)
            incr();

        return { begin, length };
    }
}