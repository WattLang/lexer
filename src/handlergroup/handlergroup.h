#pragma once


#include <array>
#include <initializer_list>
#include <utility>

#include <alias.h>


namespace ws::lexer {
    struct HandlerGroup {
        std::array<alias::Handler, 256> entries;

        constexpr HandlerGroup(
            const std::initializer_list<std::pair<const char*, const alias::Handler>>& entries_
        ) noexcept :
            entries{{nullptr}}
        {
            for (const auto& x: entries_) {
                insert(x.first, x.second);
            }
        }




        constexpr auto begin() const noexcept { return entries.begin(); }
        constexpr auto end()   const noexcept { return entries.end();   }




        constexpr const alias::Handler& at(const unsigned long& i) const {
            return entries.at(i);
        }


        constexpr const alias::Handler& operator[](const unsigned long& i) const {
            return entries[i];
        }


        constexpr void insert (
            const char* chars,
            const alias::Handler& handler
        ) noexcept {
            unsigned long i = 0;
            char p = chars[i];

            while (p != '\0') {
                entries[static_cast<unsigned int>(p)] = handler;
                p = chars[i++];
            }
        }
    };
}