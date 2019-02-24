#pragma once


#include <string_view>
#include "../alias.h"


namespace watt::lexer {
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
			char next() noexcept { return *current++; }

			void incr(int i) noexcept { current += i; }
			void decr(int i) noexcept { current -= i; }

			void incr() noexcept { ++current; }
			void dect() noexcept { --current; }


			// View characters.
			char peek(int i) const noexcept { return *(current + i); }
			char peek() const noexcept { return *current; }



			// Info.
			buffer_type::size_type size() const noexcept {
				return buffer.size();
			}


			constexpr auto end() const noexcept {
				return buffer.end();
			}


			constexpr auto begin() const noexcept {
				return buffer.begin();
			}


			bool is_end() const noexcept {
				return current >= end();
			}



			// Conditional consume.
			bool match(char expect) noexcept {
				return (peek(1) == expect) ?
					(incr(), true)
					: false;
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
