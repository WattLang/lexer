#include "lexer.h"

#include <stdexcept>
#include <string>

#include <constant.h>
#include <exception.h>

#include <libs/module.h>


namespace ws::lexer {
    int lexer(
        StringIter& iter,
        Serializer& out,
        const HandlerGroup& handlers
    ) {
        try {
            char c;

            try {
                while (not iter.is_end()) {
                    c = iter.peek();
                    const auto& handler = handlers.at(static_cast<unsigned long>(c));


                    if (not handler)
                        throw std::out_of_range("character has no valid handler.");


                    // handler is valid.
                    handler(iter, out);

                    iter.next();
                }


            } catch (const std::out_of_range&) {
                throw exception::LexicalInternalError(
                    "cannot find associated handler for '" + std::string{c} + "' character."
                );


            } catch (const exception::LexicalNotice& e) {
                ws::module::noticeln(
                    "lexical notice occured: ",
                    ws::module::lines(1), ws::module::tabs(1),
                    e.get_msg()
                );

                return 1;


            } catch (const exception::LexicalWarn& e) {
                ws::module::warnln(
                    "lexical warn occured: ",
                    ws::module::lines(1), ws::module::tabs(1),
                    e.get_msg()
                );

                return 2;


            } catch (const exception::LexicalError& e) {
                ws::module::errorln(
                    "lexical error occured: ",
                    ws::module::lines(1), ws::module::tabs(1),
                    e.get_msg()
                );

                return 3;
            }

        } catch (const exception::LexicalInternalError& e) {
            ws::module::errorln(
                "internal error occured: ",
                ws::module::lines(1), ws::module::tabs(1),
                e.get_msg()
            );

            return 4;
        }


        return 0;
    }
}