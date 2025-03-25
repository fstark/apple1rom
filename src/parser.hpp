#pragma once

#include <memory>
#include <string>
#include <vector>

#include "rom512.hpp"
#include "stringtokenizer.hpp"

class parser
{
    rom512 &rom_;
    std::shared_ptr<stringtokenizer> tokenizer_;

   public:
    parser(rom512 &rom, std::shared_ptr<stringtokenizer> tokenizer)
        : rom_(rom), tokenizer_(std::move(tokenizer))
    {
    }
    parser(rom512 &rom, std::string &source)
        : rom_(rom), tokenizer_(std::make_shared<stringtokenizer>(chartokenizer::create(source)))
    {
        parse();
    }

   private:
    void parse();
    void parse_copy();
    void parse_copy_file();
    void parse_copy_data();
    bool parse_anyadrs(uint8_t &page, uint16_t &adrs);
};

/// Parses a file containing ROM entry specifications.
void parse_romspec(const std::string &filename, rom512 &rom);
