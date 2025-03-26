#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "chartokenizer.hpp"
#include "romentryspec.hpp"
#include "stringtokenizer.hpp"

void parse_romspec(const std::string &filename, rom512 &rom)
{
    // Read all the file into a string
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_content = buffer.str();

    parser p(rom, file_content);
}

bool parser::parse_anyadrs(uint8_t &page, uint16_t &adrs)
{
    // Get the address
    auto paged_adrs_str = tokenizer_->next_string();

    //  format is nn:nnnn, with nn hexadecimal
    //  extract into page and address

    auto colon_pos = paged_adrs_str.find(':');
    if (colon_pos == std::string::npos)
    {
        adrs = static_cast<uint16_t>(std::stoi(paged_adrs_str, nullptr, 16));
        return false;
    }
    else
    {
        page = static_cast<uint8_t>(std::stoi(paged_adrs_str.substr(0, colon_pos), nullptr, 16));
        adrs = static_cast<uint16_t>(std::stoi(paged_adrs_str.substr(colon_pos + 1), nullptr, 16));
        return true;
    }
}

void parser::parser_copy_to(std::vector<uint8_t> data)
{
    std::string token = tokenizer_->next_string();

    pagedadrs_t final_adrs{0, adrs_t{0}};
    if (token == "TO")
    {
        uint8_t page;
        uint16_t adrs;
        if (parse_anyadrs(page, adrs))
        {
            final_adrs = pagedadrs_t(page, adrs_t{adrs});
        }
        else
        {
            if (!rom_.find_space(data.size(), adrs_t{adrs}, final_adrs))
            {
                throw std::runtime_error("Cannot find " + std::to_string(data.size()) +
                                         " bytes starting at " + std::to_string(adrs) +
                                         " in any page");
            }
        }
    }
    else if (token == "ANYWHERE")
    {
        if (!rom_.find_space(data.size(), final_adrs))
        {
            throw std::runtime_error("Cannot find " + std::to_string(data.size()) +
                                     " bytes anywhere");
        }
    }
    else
        throw std::runtime_error("Expected TO or ANYWHERE, got: " + token);

    rom_.store(data, final_adrs);
    last_pagedadrs_ = final_adrs;
}

// COPY FILE BOOT.BIN TO 00:2000
void parser::parse_copy_file()
{
    auto filename = tokenizer_->next_string();
    // Read file into data
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
    file.close();

    parser_copy_to(data);
}

// COPY DATA 00 TO 00:9FFF
void parser::parse_copy_data()
{
    // Get the data
    std::vector<uint8_t> data;
    std::string data_str = tokenizer_->next_string();
    for (size_t i = 0; i < data_str.size(); i += 2)
    {
        std::string byte_str = data_str.substr(i, 2);
        data.push_back(std::stoi(byte_str, nullptr, 16));
    }

    parser_copy_to(data);
}

void parser::parse_copy()
{
    // Get the type
    std::string type = tokenizer_->next_string();

    if (type == "FILE")
        parse_copy_file();
    else if (type == "DATA")
        parse_copy_data();
}

void parser::parse_menu() { last_menu_name_ = tokenizer_->next_string(); }

void parser::parse_exec()
{
    menu_.push_back(menu_item(last_menu_name_, execaction{last_pagedadrs_}));
}

void parser::parse()
{
    // Reserve space for the code4096 bytes for the menu
    rom_.reserve(pagedadrs_t(0, adrs_t(0x2100)), 4096);

    // Loop over all the tokens and process them
    while (!tokenizer_->peek_string().empty())
    {
        auto str = tokenizer_->next_string();
        if (str == "COPY") parse_copy();
        if (str == "MENU") parse_menu();
        if (str == "EXEC") parse_exec();
    }

    std::clog << "Parsed " << menu_.size() << " menu items" << std::endl;
    emiter e;
    for (const auto &item : menu_)
    {
        std::clog << "Menu item [" << item.name() << "]" << std::endl;
        for (const auto &action : item.actions())
        {
            action->doit(e);
        }
    }

    if (e.code().size() > 4096)
    {
        throw std::runtime_error("Code too big");
    }
    rom_.store_unchecked(e.code(), pagedadrs_t(0, adrs_t(0x2100)));
}
