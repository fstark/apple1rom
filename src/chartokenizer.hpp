#pragma once

#include <string>

class chartokenizer
{
   public:
    enum TokenType
    {
        CHAR,
        QUOTE,
        INVALID,
        EOS
    };

    struct Token
    {
        TokenType type;
        char value;
    };

    chartokenizer(const std::string& str);

    Token peek_token();
    Token next_token();

   private:
    std::string str_;
    size_t pos_;
    size_t next_pos_;
};
