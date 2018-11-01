#pragma once


#include <string>
#include <alias.h>


namespace ws::lexer {
    struct StringIter {
        std::string_view buffer;
		const char *current = 0;

        StringIter(const std::string_view& buffer_): buffer(buffer_), current(buffer.data()) {}



		// Iterate.
		char next() { return *current++; }
		char prev() { return *current--; }

		char next(const unsigned i) {
			current += i;
			return *(current - i);
		}

		char prev(const unsigned i) {
			current -= i;
			return *(current + i);
		}


        void incr() noexcept { current++; }
        void decr() noexcept { current--; }

        void incr(const unsigned i) noexcept { current += i; }
        void decr(const unsigned i) noexcept { current -= i; }









        // View characters.
        char at(const unsigned i)   const { return buffer.at(i);    }
        char peek(const unsigned i) const { return *(current + i);	}
        char peek()             const { return *current;			}






        // Info.
        std::string::size_type size() const noexcept { return buffer.size(); }

        bool is_end() const noexcept {
            return current >= (buffer.data() + size());
        }








        // Read N characters.
        const std::string_view read(const std::string::size_type& n) const {
            return buffer.substr(current - buffer.data() + 1, n);
        }



        // Check if next character is expected and advance.
        bool match(char expect);


        // Skip over characters until a certain condition is met.
        void next_while(const alias::WhilePred& pred);


        const std::string_view read_while(const alias::WhilePred& pred);
    };
}