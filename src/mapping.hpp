#pragma once

#include <memory>
#include <vector>

#include "pagedadrs.hpp"
#include "rom512.hpp"
#include "romentryspec.hpp"

using namespace std::string_literals;

/// A mapping between a set of romentry spec and a rom512
class mapping
{
    typedef struct
    {
        std::shared_ptr<romentryspec> entry_;
        pagedadrs_t adrs_;
    } entrymap;

    std::vector<entrymap> entries_;

    rom512 &rom_;

    /// Finds somewhere in the rom a place to store the data
    pagedadrs_t find_adrs(const romentryspec &entry)
    {
        std::clog << "Finding adrs for entry " << entry.name() << std::endl;

        if (entry.type() == romentryspec::kFixed)
        {
            return pagedadrs_t(0, entry.adrs());  // todo find bank
        }

        if (entry.type() == romentryspec::kRelocatable)
        {
            return rom_.allocate(entry.data().size());
        }

        throw std::runtime_error("Unable to find adrs for entry "s + entry.name());
    }

   public:
    mapping(rom512 &rom) : rom_(rom) {}

    void add_entry(std::shared_ptr<romentryspec> entry)
    {
        entrymap em{entry, pagedadrs_t(0, adrs_t(0))};
        entries_.push_back(em);

        auto adrs = find_adrs(*entry);

        std::clog << "Storing " << entry->data().size() << " bytes at " << adrs.to_string()
                  << std::endl;
        rom_.store(entry->data(), adrs);
    }
};
