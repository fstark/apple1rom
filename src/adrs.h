#pragma once

#include <stdint.h>

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

private:
    uint16_t address_;
};
