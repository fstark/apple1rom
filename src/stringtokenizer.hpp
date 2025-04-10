// This header file defines the `stringtokenizer` class, responsible for tokenizing strings using a
// `chartokenizer`. It provides methods to peek and retrieve strings, as well as accept specific
// strings.
#pragma once

#include <memory>  // For std::shared_ptr
#include <stdexcept>
#include <string>

#include "chartokenizer.hpp"

class stringtokenizer
{
   public:
    // Initializes the `stringtokenizer` with a `chartokenizer` instance.
    stringtokenizer(std::shared_ptr<chartokenizer> char_tok);

    // Peeks at the next string without consuming it.
    std::string peek_string();
    // Retrieves the next string and advances the tokenizer.
    std::string next_string();
    // Accepts a specific string, ensuring it matches the current token.
    void accept(const std::string &str);

   private:
    // Extracts the current string from the tokenizer.
    void extract_current_string();

    // Private members for managing the `chartokenizer` and current string state.
    std::shared_ptr<chartokenizer> char_tok_;
    std::string current_string_;
    bool has_peeked_;
};
