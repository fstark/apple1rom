// This header file defines the `chartokenizer` class, responsible for tokenizing characters in a
// string. It provides functionality to peek and retrieve tokens, as well as create instances of the
// tokenizer.
#pragma once

#include <memory>
#include <string>

class chartokenizer
{
   public:
    // Enum representing the type of token (e.g., character, quote, invalid, or end of string).
    enum TokenType
    {
        CHAR,
        QUOTE,
        INVALID,
        EOS
    };

    // Struct representing a token with its type and value.
    struct Token
    {
        TokenType type;
        char value;
    };

    // Initializes the tokenizer with a string.
    chartokenizer(const std::string& str);

    // Peeks at the next token without consuming it.
    Token peek_token();
    // Retrieves the next token and advances the tokenizer.
    Token next_token();

    // Resets the tokenizer to its initial state.
    void reset();

    // Creates a shared pointer to a `chartokenizer` instance.
    static std::shared_ptr<chartokenizer> create(const std::string& str);

   private:
    // Private members for managing the string and positions.
    std::string str_;
    size_t pos_;
    size_t next_pos_;
};
