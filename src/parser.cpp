#include "parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<romentryspec> parse_romentryspecs(const std::string& filename) {
    std::vector<romentryspec> entries;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name, type, binfile;
        uint16_t address;

        if (!(iss >> name >> type >> binfile >> std::hex >> address)) {
            throw std::runtime_error("Invalid format in file: " + filename);
        }

        //  Read binfile into data vector
        std::vector<uint8_t> data;
        std::ifstream bin(binfile, std::ios::binary);
        if (!bin.is_open()) {
            throw std::runtime_error("Could not open file: " + binfile);
        }
        data = std::vector<uint8_t>(std::istreambuf_iterator<char>(bin), {});

        romentryspec entry(
            name,
            data,
            adrs_t(address),
            pagedadrs_t(0, adrs_t(address)) );

        entries.push_back(entry);
    }

    return entries;
}
