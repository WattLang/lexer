#pragma once


#include <stringiter/stringiter.h>
#include <serializer/serializer.h>
#include <handlergroup/handlergroup.h>


namespace ws::lexer {
    int lexer(
        StringIter& iter,
        Serializer& out,
        const HandlerGroup& handlers
    );
}