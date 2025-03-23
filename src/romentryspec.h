#pragma once

#include "pagedadrs.h"

#include <string>
#include <vector>

///A romentryspec is a specification of a ROM entry.
class romentryspec
{
    /// The name of the ROM entry.
    std::string name_;
    /// The data of the ROM entry.
    std::vector<uint8_t> data_;

    /// The execution address
    adrs_t exec_;
    /// The allocated address into the ROM.
    pagedadrs_t addr_;

public:
    romentryspec(
        const std::string& name,
        const std::vector<uint8_t>& data,
        adrs_t exec,
        pagedadrs_t addr
    ) :
       name_(name),
        data_(data),
        exec_(exec),
        addr_(0,adrs_t(0))
        {}
};

