// This header file defines the `adrs_t` class, encapsulating a 16-bit address.
// It provides various utility methods for address manipulation, comparison, and conversion to
// string.
#pragma once

#include <stdint.h>

#include <compare>
#include <iomanip>
#include <sstream>

/// A class that encapsulates a uint16_t address.
class adrs_t
{
   public:
    /// Initializes the address with a uint16_t value.
    explicit adrs_t(uint16_t address) : address_(address) {}

    /// Explicitly extracts the underlying address.
    explicit operator uint16_t() const { return address_; }

    /// Adds or subtracts an offset to/from the address.
    adrs_t operator+(int offset) const { return adrs_t(address_ + offset); }
    adrs_t operator-(int offset) const { return adrs_t(address_ - offset); }

    /// Substraction with another address
    int operator-(const adrs_t& other) const { return address_ - other.address_; }

    /// Compares addresses.
    auto operator<=>(const adrs_t&) const = default;

    /// Inequality operator
    bool operator!=(const adrs_t& other) const { return address_ != other.address_; }

    /// += operator
    adrs_t& operator+=(size_t len)
    {
        address_ += len;
        return *this;
    }

    /// Retrieves the low byte of the address.
    uint8_t get_low() const { return address_ & 0xff; }

    /// Retrieves the high byte of the address.
    uint8_t get_high() const { return address_ >> 8; }

    /// Converts the address to a hexadecimal string.
    std::string to_string() const
    {
        std::stringstream ss;
        ss << std::hex << std::setw(4) << std::setfill('0') << address_;
        return ss.str();
    }

   private:
    /// Private member to store the 16-bit address.
    uint16_t address_;
};
