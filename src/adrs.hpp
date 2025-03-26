#pragma once

#include <stdint.h>

#include <compare>
#include <iomanip>
#include <sstream>

/// A class that encapsulates a uint16_t address.
class adrs_t
{
   public:
    /// Constructor that initializes the address with a uint16_t value.
    explicit adrs_t(uint16_t address) : address_(address) {}

    /// Explicitly extract the underlying address.
    explicit operator uint16_t() const { return address_; }

    /// Addition operator with an int
    adrs_t operator+(int offset) const { return adrs_t(address_ + offset); }
    adrs_t operator-(int offset) const { return adrs_t(address_ - offset); }

    /// Substraction with another address
    int operator-(const adrs_t& other) const { return address_ - other.address_; }

    /// Comparison (using spaceship operator)
    auto operator<=>(const adrs_t&) const = default;

    /// Low byte
    uint8_t get_low() const { return address_ & 0xff; }

    /// High byte
    uint8_t get_high() const { return address_ >> 8; }

    /// To string
    std::string to_string() const
    {
        std::stringstream ss;
        ss << std::hex << std::setw(4) << std::setfill('0') << address_;
        return ss.str();
    }

   private:
    uint16_t address_;
};
