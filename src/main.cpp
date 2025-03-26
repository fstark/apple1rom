#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

#include "mapping.hpp"
#include "parser.hpp"
#include "rom512.hpp"

using namespace std::string_literals;

int main(int argc, const char** argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <input filename> <output filename>\n", argv[0]);
        return 1;
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];

    rom512 rom;

    /// Parse entry specs
    try
    {
        parse_romspec(input_filename, rom);
    }
    catch (const std::string& error)
    {
        std::cerr << "Error: " << error << std::endl;
        return 1;
    }

    /*
        mapping map(rom);

            for (const auto& entry : entries)
        {
            std::clog << "Entry [" << entry->name() << "] at " << entry->adrs().to_string()
                      << std::endl;
            map.add_entry(entry);  // #### Maybe we should pass the rom
        }
    */

    std::clog << "Generating [" + output_filename + "]" << std::endl;

    // Write content to output_filename
    auto content = rom.get_content();
    std::ofstream output_file(output_filename, std::ios::binary);
    if (!output_file)
    {
        std::cerr << "Error: Could not open output file " << output_filename << std::endl;
        return 1;
    }
    output_file.write(reinterpret_cast<const char*>(content.data()), content.size());
    output_file.close();

    return 0;
}
