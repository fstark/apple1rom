#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "asm.hpp"
#include "chartokenizer.hpp"
#include "stringtokenizer.hpp"

static uint8_t uint8_from_string(const std::string &s)
{
    size_t index;
    uint8_t res;
    try
    {
        res = static_cast<uint8_t>(std::stoi(s, &index, 16));
    }
    catch (const std::invalid_argument &e)
    {
        throw std::runtime_error("Invalid number: " + s);
    }
    if (index != s.size())
    {
        throw std::runtime_error("Invalid number: " + s);
    }
    return res;
}

static uint16_t uint16_from_string(const std::string &s)
{
    size_t index;
    uint16_t res;
    try
    {
        res = static_cast<uint16_t>(std::stoi(s, &index, 16));
    }
    catch (const std::invalid_argument &e)
    {
        throw std::runtime_error("Invalid number: " + s);
    }
    if (index != s.size())
    {
        throw std::runtime_error("Invalid number: " + s);
    }
    return res;
}

static adrs_t adrs_from_string(const std::string &str) { return adrs_t{uint16_from_string(str)}; }

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

adrs_t parser::parse_adrs0()
{
    // Get the address
    auto adrs_str = tokenizer_->next_string();

    //  format is nnnn, with nn hexadecimal
    return adrs_from_string(adrs_str);
}

void parser::parse_adrs0_len(adrs_t &adrs, size_t &len)
{
    // Get the address
    auto adrs_str = tokenizer_->next_string();

    adrs = adrs_from_string(adrs_str);

    auto len_str = tokenizer_->peek_string();
    size_t index = 0;
    size_t parsed_len;
    try
    {
        parsed_len = std::stoi(len_str, &index, 16);
    }
    catch (const std::invalid_argument &e)
    {
        //  Not a number at all
        //  index will be 0, so we'll skip next block
    }
    if (index == len_str.size())
    {
        len = parsed_len;
        tokenizer_->next_string();
        std::clog << "found len == " << len << std::endl;
    }

    std::clog << "ADRS LEN :" << adrs.to_string() << " len == " << len << std::endl;
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
        adrs = (uint16_t)adrs_from_string(paged_adrs_str);
        return false;
    }
    else
    {
        page = uint8_from_string(paged_adrs_str.substr(0, colon_pos));
        adrs = uint16_from_string(paged_adrs_str.substr(colon_pos + 1));
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
    last_pagedadrs_ = last_pagedadrs_original_ = final_adrs;
    last_len_ = last_len_original_ = data.size();
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
        data.push_back(uint8_from_string(byte_str));
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

void parser::parse_menu()
{
    last_menu_name_ = tokenizer_->next_string();
    last_action_ = std::make_shared<menu_action>(last_menu_name_);
    root_menu_.add_item_path(last_action_);
    // all_menu_actions_.push_back(std::make_shared<menu_action>(
    //     last_menu_name_, execaction{pagedadrs_t(page, adrs_t(adrs))}));
}

void parser::parse_exec()
{
    uint8_t page;
    uint16_t adrs;
    if (!parse_anyadrs(page, adrs))
    {
        page = last_pagedadrs_original_.get_page();
        std::clog << "EXEC USING IMPLICIT PAGE " << (int)page << " FOR " << last_menu_name_
                  << std::endl;
    }
    last_action_->add_action(std::make_shared<execaction>(pagedadrs_t(page, adrs_t(adrs))));
}

void parser::parse_load()
{
    //    auto adrs = parse_adrs0();
    auto len = last_len_;
    adrs_t adrs{0};
    parse_adrs0_len(adrs, len);
    last_action_->add_action(std::make_shared<loadaction>(adrs, last_pagedadrs_, len));
    last_len_ -= len;
    last_pagedadrs_ += len;
}

void parser::process_tokens(bool calculate_size, emiter* temp_emiter)
{
    while (!tokenizer_->peek_string().empty())
    {
        auto str = tokenizer_->next_string();
        if (str == "COPY") parse_copy();
        if (str == "MENU") parse_menu();
        if (str == "EXEC") parse_exec();
        if (str == "LOAD") parse_load();
    }

    if (calculate_size && temp_emiter)
    {
        root_menu_.emit(*temp_emiter);
        calculated_menu_size_ = temp_emiter->code().size();
    }
}

void parser::calculate_menu_size()
{
    emiter temp_emiter((adrs_t)ROM0MENU);
    process_tokens(true, &temp_emiter);
}

void parser::parse()
{
    // First pass: Calculate the size of the menu
    calculate_menu_size();

    // Check if the calculated size exceeds the limit
    if (calculated_menu_size_ > 4096)
    {
        throw std::runtime_error("Menu size exceeds the maximum limit of 4096 bytes");
    }

    // Second pass: Reserve the required space and parse the menu
    rom_.reserve(pagedadrs_t(0, ROM0MENU), calculated_menu_size_);

    // Reset the tokenizer for the second pass
    tokenizer_->reset();

    // Process tokens for the second pass
    process_tokens(false, nullptr);

    // Emit the root menu
    emiter e((adrs_t)ROM0MENU);
    root_menu_.emit(e);

    // Store the emitted code in ROM
    rom_.store_unchecked(e.code(), pagedadrs_t(0, ROM0MENU));
}
