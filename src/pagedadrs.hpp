#pragma once

#include <stdint.h>

#include <iomanip>
#include <stdexcept>

#include "adrs.hpp"

/// A class that encapsulates a page (0 to 15) and an adrs_t.
class pagedadrs_t
{
    uint8_t page_;
    adrs_t address_;

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

    /// To string
    std::string to_string() const
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(page_) << ":"
           << address_.to_string();
        return ss.str();
    }

    // += operator (only operates on the address)
    pagedadrs_t &operator+=(size_t len)
    {
        address_ += len;
        return *this;
    }
};
