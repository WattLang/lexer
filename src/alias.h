#pragma once


#include <vector>
#include <string_view>
#include <functional>
#include "libs/position.h"


namespace ws::lexer {
	struct StringIter;

	template <typename T>
	using Group	   = std::vector<T>;
	using Position = ws::structure::Position;

	using WhilePred = bool(*const)(StringIter&, const char);

	template <typename T>
	using Rule = void(*)(StringIter&, Group<T>&);
}
