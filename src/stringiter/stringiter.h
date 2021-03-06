#pragma once


#include <string_view>
#include "../alias.h"


namespace ws::lexer {
	class StringIter {

		using ptr_type = const char*;
		using buffer_type = std::string_view;


		private:

			// Data.
			buffer_type buffer;
			ptr_type current = nullptr;



		public:

			// Constructor.
			StringIter(buffer_type buffer_):
				buffer(buffer_), current(buffer.data())
			{

			}



			// Getters.
			ptr_type ptr() const noexcept { return current; }



			// Iterate.
			char next()		  noexcept { return *current++; }
			void incr(int i = 1) noexcept { current += i;	  }
			void decr(int i = 1) noexcept { current -= i;	  }



			// View characters.
			char peek(int i = 0) const noexcept { return *(current + i); }



			// Info.
			buffer_type::size_type size() const noexcept {
				return buffer.size();
			}

			bool is_end(int offset = 0) const noexcept {
				return current >= ((buffer.data() + size()) + offset);
			}



			// Conditional consume.
			bool match(char expect) noexcept {
				auto b = (peek(1) == expect);

				// If we have a match, increment the current ptr.
				if (b) incr();

				return b;
			}


			// Skip characters until predicate fails.
			void skip_while(WhilePred pred) {
				while (pred(*this, peek(1)) and not is_end())
					incr();
			}


			// Consume characters while predicate is satisfied.
			buffer_type read_while(WhilePred pred) {
				ptr_type begin = current;
				std::string::size_type length = 1;

				for (; pred(*this, peek(1)) and not is_end(); ++length)
					incr();

				return { begin, length };
			}
	};
}
