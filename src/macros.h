#pragma once


#include "constants.h"


#define WS_LEXER_DEBUG(code) if constexpr(ws::lexer::ENABLE_DEBUG) { code; }