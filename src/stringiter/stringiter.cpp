#include "stringiter.h"


#include <constant.h>


namespace ws::lexer {
    // Check if next character is expected and advance.
    bool StringIter::match(char expect) {
        if (is_end() or peek(1) != expect)
            return false;


        incr(2);
        return true;
    }


    // Skip over characters until a certain condition is met.
    void StringIter::next_while(const alias::WhilePred& pred) {
        while (true) {
            // If the next char is not valid, break and return builder.

            // The size is offset because the predicate must look forward by size + 1 in the case of the final character which would lead to an exception.

            // (Expression order is important here.)
            if (current >= (size() - 1) or not pred(*this, peek(1)))
                break;

            incr();
        }
    }


    const std::string StringIter::read_while(const alias::WhilePred& pred) {
        static std::string builder(constant::STRING_BUFFER_SIZE_INITIAL, '\0');
        builder.clear();
        std::string::size_type ptr = 0;


        while (true) {
            if (ptr % constant::STRING_BUFFER_SIZE == 0) {
                builder.resize(builder.size() + constant::STRING_BUFFER_SIZE);
            }

            //builder += peek();
            builder[ptr] = peek();
            // If the next char is not valid, break and return builder.

            // The size is offset because the predicate must look forward by size + 1 in the case of the final character which would lead to an exception.

            // (Expression order is important here.)
            if (current >= (size() - 1) or not pred(*this, peek(1)))
                break;

            incr();
            ++ptr;
        }


        return builder;
    }

}