#pragma once

#include <memory>
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

    static std::shared_ptr<chartokenizer> create(const std::string& str);

   private:
    std::string str_;
    size_t pos_;
    size_t next_pos_;
};
