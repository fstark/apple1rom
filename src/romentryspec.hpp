#pragma once

#include <string>
#include <vector>

#include "pagedadrs.hpp"

/// A romentryspec is a specification of a ROM entry.
class romentryspec
{
   public:
    typedef enum
    {
        kFixed = 0,
        kRelocatable = 1
    } eType;

   protected:
    /// The name of the ROM entry.
    std::string name_;
    /// The data of the ROM entry.
    std::vector<uint8_t> data_;

    /// The address
    adrs_t adrs_;

    /// The paged address
    pagedadrs_t pagedadrs_;

    /// Type of entry
    eType type_;

   public:
    romentryspec(const std::string& name, const std::vector<uint8_t>& data, adrs_t adrs,
                 pagedadrs_t pagedadrs, eType type)
        : name_(name), data_(data), adrs_(adrs), pagedadrs_(pagedadrs), type_(type)
    {
    }

    //  Name
    const std::string& name() const { return name_; }

    //  Data
    const std::vector<uint8_t>& data() const { return data_; }

    //  Adrs
    const adrs_t& adrs() const { return adrs_; }

    //  Paged Adrs
    const pagedadrs_t& pagedadrs() const { return pagedadrs_; }

    /// Entry type
    eType type() const { return type_; }
};
