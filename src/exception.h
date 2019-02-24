#pragma once


#include <string>
#include <stdexcept>


#define WATT_LEXER_NEW_EXCEPTION_TYPE(name, default_msg) \
	class name: public std::runtime_error { \
		private: \
			std::string msg; \
		public: \
			name(const std::string& msg_): \
				std::runtime_error(msg_), \
				msg(msg_) \
			{ \
			} \
			\
			name(): \
				std::runtime_error(default_msg), \
				msg(default_msg) \
			{ \
			} \
			\
			std::string get_msg() const { return msg; } \
	};


namespace watt::lexer::exception {
	WATT_LEXER_NEW_EXCEPTION_TYPE(Notice, "Lexical notice!")
	WATT_LEXER_NEW_EXCEPTION_TYPE(Warn,   "Lexical warning!")
	WATT_LEXER_NEW_EXCEPTION_TYPE(Error,  "Lexical error!")
}


#undef WATT_LEXER_NEW_EXCEPTION_TYPE
