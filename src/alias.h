#pragma once


#include <vector>
#include <string_view>
#include <functional>


namespace watt::lexer {
	class StringIter;

	template <typename T>
	using Group	= std::vector<T>;

	using WhilePred = bool(*const)(StringIter&, const char);

	template <typename T>
	using Rule = void(*)(StringIter&, Group<T>&);
}
