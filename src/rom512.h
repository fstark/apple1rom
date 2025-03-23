#pragma once

#include "adrs.h"
#include <vector>
#include "romentryspec.h"
#include <array>

const adrs_t min_adrs(0x2000);
const adrs_t max_adrs(0xA000);

/// A 32K rom bank
class rombank32
{
    std::array<uint8_t,65536> data_;

    public:
    void add( const std::vector<uint8_t> data, const adrs_t adrs )
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            data_[(uint16_t)(adrs+i)] = data[i];
        }
    }
};

/// A 512K apple1 rom
class rom512
{
    std::vector<rombank32> banks_;    

public:
    void add(const romentryspec& entry, const pagedadrs_t& adrs)
    {
        // get the bank
        auto bank = banks_[adrs.get_page()];

        // add the data
        bank.add(entry.data(), adrs.get_address());
    }
};
