#pragma once

#include <array>



namespace ws::lexer {
    struct LookupTable {
        using table_t = std::array<bool, 256>;
        using size_type = table_t::size_type;

        table_t table;



        constexpr LookupTable(const char* chars, bool flag): table{{false}} {
            size_type i = 0;
            char p = chars[i];

            while (p != '\0') {
                table.at(static_cast<size_type>(p)) = flag;
                p = chars[i++];
            }
        }




        constexpr auto begin() const noexcept { return table.begin(); }
        constexpr auto end()   const noexcept { return table.end();   }





        constexpr bool at(char i) const {
            return table.at(static_cast<unsigned long>(i));
        }


        constexpr bool operator[](char i) const {
            return at(i);
        }
    };
}