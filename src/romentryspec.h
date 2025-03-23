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

public:
    romentryspec(
        const std::string& name,
        const std::vector<uint8_t>& data,
        adrs_t exec
    ) :
       name_(name),
        data_(data),
        exec_(exec)
        {}

    //  Name
    const std::string& name() const { return name_; }

    //  Data
    const std::vector<uint8_t>& data() const { return data_; }

    //  Execution Adrs
    const adrs_t& exec() const { return exec_; }
};

