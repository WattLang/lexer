#pragma once


#include <string>
#include <optional>
#include "../exception.h"
#include "../rules/rules.h"
#include "../stringiter/stringiter.h"


namespace watt::lexer {

	template <typename T>
	void consume(
		const Rules<typename T::value_type>& rules,
		watt::lexer::StringIter& iter,
		T& tokens
	) {
		rules.at(iter.peek()) (iter, tokens);
		iter.incr();
	}


	template <typename T>
	void lexer(
		const Rules<typename T::value_type>& rules,
		watt::lexer::StringIter& iter,
		T& tokens
	) {
		while (not iter.is_end())
			consume(rules, iter, tokens);
	}

}
