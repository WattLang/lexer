#pragma once


#include <array>
#include <initializer_list>
#include <utility>

#include <alias.h>


namespace ws::lexer {
    struct Rules {

        // Aliases
        using Entries = std::array<alias::Rule, 256>;


        // Data
        Entries entries;




        // Ctor
        constexpr Rules(
            const std::initializer_list<std::pair<const char*, const alias::Rule>>& entries_
        ):
            entries{{nullptr}}
        {
            // This should be just:
            // for (const auto& x: entries_)
            // but it doesn't work in MSVC (constexpr bug with lifetime of variables)
            for (auto it = entries_.begin(); it != entries_.end(); ++it) {
                insert(it->first, it->second);
            }
        }




        constexpr auto begin() const noexcept { return entries.begin(); }
        constexpr auto end()   const noexcept { return entries.end();   }




        constexpr const alias::Rule& at(const unsigned long& i) const {
            return entries.at(i);
        }


        constexpr const alias::Rule& operator[](const unsigned long& i) const {
            return entries[i];
        }


        constexpr const alias::Rule& at(char i) const {
            return entries.at(static_cast<unsigned long>(i));
        }


        constexpr const alias::Rule& operator[](char i) const {
            return at(i);
        }


        constexpr void insert (const char* chars, const alias::Rule& rule) {
            Entries::size_type i = 0;
            char p = chars[i];

            while (p != '\0') {
                entries.at(static_cast<Entries::size_type>(p)) = rule;
                p = chars[i++];
            }
        }
    };
}