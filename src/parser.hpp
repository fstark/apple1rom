#pragma once

#include <memory>
#include <string>
#include <vector>

#include "menu.hpp"
#include "rom512.hpp"
#include "stringtokenizer.hpp"

class parser
{
    rom512 &rom_;
    std::shared_ptr<stringtokenizer> tokenizer_;
    std::vector<std::shared_ptr<menu_action>> all_menu_actions_;
    std::shared_ptr<menu_action> last_action_;

    std::string last_menu_name_;
    pagedadrs_t last_pagedadrs_;
    size_t last_len_;

    menu root_menu_;

   public:
    parser(rom512 &rom, std::string &source)
        : rom_(rom),
          tokenizer_(std::make_shared<stringtokenizer>(chartokenizer::create(source))),
          last_pagedadrs_(0, adrs_t(0)),
          last_len_(0),
          root_menu_("/")
    {
        parse();
    }

   private:
    void parse();
    void parse_copy();
    void parse_copy_file();
    void parse_copy_data();
    void parser_copy_to(std::vector<uint8_t> data);
    bool parse_anyadrs(uint8_t &page, uint16_t &adrs);
    adrs_t parse_adrs0();
    void parse_menu();
    void parse_exec();
    void parse_load();
};

/// Parses a file containing ROM entry specifications.
void parse_romspec(const std::string &filename, rom512 &rom);
