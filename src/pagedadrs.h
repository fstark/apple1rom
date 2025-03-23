#pragma once

#include <stdint.h>
#include "adrs.h"
#include <stdexcept>

/// A class that encapsulates a page (0 to 15) and an adrs_t.
class pagedadrs_t
{
public:
    /// Constructor that initializes the page and address.
    pagedadrs_t(uint8_t page, adrs_t address) : page_(page), address_(address) 
    {
        if (page > 15) 
        {
            throw std::out_of_range("Page must be between 0 and 15");
        }
    }

    /// Get the page.
    uint8_t get_page() const { return page_; }

    /// Get the address.
    adrs_t get_address() const { return address_; }

private:
    uint8_t page_;
    adrs_t address_;
};
