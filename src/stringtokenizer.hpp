#pragma once

#include <stdexcept>
#include <string>

#include "chartokenizer.hpp"

class stringtokenizer
{
   public:
    stringtokenizer(chartokenizer& char_tok);

    std::string peek_string();
    std::string next_string();

   private:
    void extract_current_string();

    chartokenizer& char_tok_;
    std::string current_string_;
    bool has_peeked_;
};
