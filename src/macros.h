#pragma once


#include "constants.h"


#define WS_LEXER_DEBUG(code) if constexpr(ws::lexer::ENABLE_DEBUG) { code; }



#define WS_LOG_HEADING(msg) if constexpr(ws::lexer::ENABLE_DEBUG) { ws::module::printer << ws::module::lines(ws::lexer::HEADING_LINE_GAP); ws::module::noticeln(ws::module::style::bold, msg); }

#define WS_LOG(msg) if constexpr(ws::lexer::ENABLE_DEBUG) { ws::module::noticeln(msg); }