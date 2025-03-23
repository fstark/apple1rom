#pragma once

#include "adrs.h"
#include <vector>
#include "romentryspec.h"

const adrs_t min_adrs(0x2000);
const adrs_t max_adrs(0xA000);

/// A 32K rom bank
class rombank32
{
    std::vector<romentryspec> entries_;
};

/// A 512K apple1 rom
class rom512
{
    std::vector<rombank32> banks_;    

public:
    void add_entry(const romentryspec& entry)
    {
        
    }
};
