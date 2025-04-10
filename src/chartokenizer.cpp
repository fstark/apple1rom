#include "chartokenizer.hpp"

#include <stdexcept>

chartokenizer::chartokenizer(const std::string& str) : str_(str), pos_(0), next_pos_(0) {}

chartokenizer::Token chartokenizer::peek_token()
{
    next_pos_ = pos_;
    if (next_pos_ >= str_.size())
    {
        return {EOS, '\0'};
    }

    char ch = str_[next_pos_++];
    if (ch == '\\')
    {
        if (next_pos_ < str_.size())
        {
            ch = str_[next_pos_++];
            switch (ch)
            {
                case 'n':
                    return {CHAR, '\n'};
                case 't':
                    return {CHAR, '\t'};
                case '\\':
                    return {CHAR, '\\'};
                case '"':
                    return {CHAR, '"'};
                default:
                    throw std::runtime_error("Invalid escape sequence");
            }
        }
    }
    else if (ch == '"')
    {
        return {QUOTE, ch};
    }

    return {CHAR, ch};
}

chartokenizer::Token chartokenizer::next_token()
{
    Token token = peek_token();
    if (token.type != EOS)
    {
        pos_ = next_pos_;
    }
    return token;
}

void chartokenizer::reset()
{
    // Reset the position to the start of the string
    pos_ = 0;
    next_pos_ = 0;
}

std::shared_ptr<chartokenizer> chartokenizer::create(const std::string& str)
{
    return std::make_shared<chartokenizer>(str);
}
