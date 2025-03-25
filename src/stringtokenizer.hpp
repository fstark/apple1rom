#pragma once

#include <memory>  // For std::shared_ptr
#include <stdexcept>
#include <string>

#include "chartokenizer.hpp"

class stringtokenizer
{
   public:
    stringtokenizer(std::shared_ptr<chartokenizer> char_tok);

    std::string peek_string();
    std::string next_string();
    void accept(const std::string &str)
    {
        if (str != next_string())
        {
            throw std::runtime_error("Expected " + str);
        }
    }

   private:
    void extract_current_string();

    std::shared_ptr<chartokenizer> char_tok_;
    std::string current_string_;
    bool has_peeked_;
};
