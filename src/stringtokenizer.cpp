#include "stringtokenizer.hpp"

#include <stdexcept>

stringtokenizer::stringtokenizer(std::shared_ptr<chartokenizer> char_tok)
    : char_tok_(char_tok), has_peeked_(false)
{
}

void stringtokenizer::extract_current_string()
{
    current_string_.clear();
    chartokenizer::Token token;

    // Skip leading spaces
    do
    {
        token = char_tok_->next_token();
    } while (token.type != chartokenizer::EOS && isspace(token.value));

    if (token.type == chartokenizer::QUOTE)
    {
        token = char_tok_->next_token();
        while (token.type != chartokenizer::EOS && token.type != chartokenizer::QUOTE)
        {
            current_string_ += token.value;
            token = char_tok_->next_token();
        }

        if (token.type != chartokenizer::QUOTE)
        {
            throw std::runtime_error("Unclosed quoted string");
        }
    }
    else
    {
        while (token.type != chartokenizer::EOS && !isspace(token.value))
        {
            current_string_ += token.value;
            token = char_tok_->next_token();
        }
    }
}

std::string stringtokenizer::peek_string()
{
    if (!has_peeked_)
    {
        extract_current_string();
        has_peeked_ = true;
    }

    return current_string_;
}

std::string stringtokenizer::next_string()
{
    if (!has_peeked_)
    {
        extract_current_string();
    }

    has_peeked_ = false;
    return current_string_;
}

void stringtokenizer::accept(const std::string &str)
{
    if (str != next_string())
    {
        throw std::runtime_error("Expected " + str);
    }
}

void stringtokenizer::reset()
{
    // Reset the internal state of the tokenizer
    char_tok_->reset();
    current_string_.clear();
    has_peeked_ = false;
}
