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

    tokenizer_->accept("TO");

    auto adrs = parse_pagedadrs();

    // Store the data in ROM
    rom_.store(data, adrs);
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

    //  log data
    // std::clog << "Data: ";
    // for (auto byte : data)
    // {
    //     std::clog << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte)
    //     << "
    //     ";
    // }
    // std::clog << std::endl;

    tokenizer_->accept("TO");

    auto adrs = parse_pagedadrs();

    // Store the entry
    rom_.store(data, adrs);
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

void parser::parse()
{
    // Loop over all the tokens and process them
    while (!tokenizer_->peek_string().empty())
    {
        auto str = tokenizer_->next_string();
        if (str == "COPY") parse_copy();
    }
}
