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

std::vector<std::shared_ptr<romentryspec>> parse_romentryspecs(const std::string& filename)
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

    // Create a chartokenizer and stringtokenizer
    chartokenizer char_tok(file_content);
    stringtokenizer str_tok(char_tok);

    std::vector<std::shared_ptr<romentryspec>> entries;

    // Loop over all the tokens and process them
    while (true)
    {
        try
        {
            std::string token = str_tok.next_string();
            if (token.empty())
            {
                break;
            }
            // Assuming romentryspec has a constructor that takes a string
            // auto entry = std::make_shared<romentryspec>(token);
            // entries.push_back(entry);
            std::cout << "[" << token << "]" << std::endl;
        }
        catch (const std::runtime_error& e)
        {
            // Handle unclosed quoted strings or other errors
            throw std::runtime_error("Error parsing file: " + std::string(e.what()));
        }
    }
    return entries;
}
