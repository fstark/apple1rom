#pragma once

#include <array>
#include <iostream>
#include <vector>

#include "adrs.hpp"
#include "romentryspec.hpp"

using namespace std::string_literals;

const int BankCount = 16;

const adrs_t min_adrs(0x2000);
const adrs_t max_adrs(0xA000);

/// A 32K rom bank
class rombank32
{
    std::array<uint8_t, 65536> data_;

    typedef struct
    {
        adrs_t start;
        size_t size;
    } freespace;

    std::vector<freespace> free_;

    // Delete copy constructor and copy assignment operator
    // rombank32(const rombank32&) = delete;
    // rombank32& operator=(const rombank32&) = delete;

   public:
    rombank32() : free_{{(adrs_t)(0x2000), 32768}} {}

    bool check_free(adrs_t adrs, size_t size) const
    {
        //  Check that the data is available
        //  (note that free blocks are not contiguous)
        for (const auto& free_block : free_)
        {
            if (adrs >= free_block.start && adrs + size <= free_block.start + free_block.size)
            {
                // The data fits within this free block
                return true;
            }
        }
        return false;
    }

    void reserve(adrs_t adrs, size_t size)
    {
        //  Checks free
        if (!check_free(adrs, size))
            throw std::runtime_error("Not enough space to reserve data at the given address");

        // Update free list
        for (size_t i = 0; i < free_.size(); ++i)
        {
            if (adrs >= free_[i].start && adrs + size <= free_[i].start + free_[i].size)
            {
                // Split the free block into two if necessary
                if (adrs > free_[i].start)
                {
                    free_.insert(free_.begin() + i,
                                 {free_[i].start, (size_t)(adrs - free_[i].start)});
                    ++i;  // Adjust index due to insertion
                }
                if (adrs + size < free_[i].start + free_[i].size)
                {
                    free_.insert(
                        free_.begin() + i + 1,
                        {adrs + size, (size_t)((free_[i].start + free_[i].size) - (adrs + size))});
                }
                // Remove the original free block
                free_.erase(free_.begin() + i);
                break;
            }
        }
    }

    void store_unchecked(const std::vector<uint8_t> data, const adrs_t adrs)
    {
        // std::clog << "Storing " << data.size() << " bytes at " << adrs.to_string() << std::endl;
        for (size_t i = 0; i < data.size(); i++)
        {
            data_[(uint16_t)(adrs + i)] = data[i];
        }
    }

    void store(const std::vector<uint8_t> data, const adrs_t adrs)
    {
        //  Checks free
        if (!check_free(adrs, data.size()))
            throw std::runtime_error("Not enough space to store data at the given address");

        store_unchecked(data, adrs);
        reserve(adrs, data.size());

        // // Update free list
        // for (size_t i = 0; i < free_.size(); ++i)
        // {
        //     if (adrs >= free_[i].start && adrs + data.size() <= free_[i].start + free_[i].size)
        //     {
        //         // Split the free block into two if necessary
        //         if (adrs > free_[i].start)
        //         {
        //             free_.insert(free_.begin() + i,
        //                          {free_[i].start, (size_t)(adrs - free_[i].start)});
        //             ++i;  // Adjust index due to insertion
        //         }
        //         if (adrs + data.size() < free_[i].start + free_[i].size)
        //         {
        //             free_.insert(free_.begin() + i + 1,
        //                          {adrs + data.size(), (size_t)((free_[i].start + free_[i].size) -
        //                                                        (adrs + data.size()))});
        //         }
        //         // Remove the original free block
        //         free_.erase(free_.begin() + i);
        //         break;
        //     }
        // }
    }

    //  Find the first space that has size bytes available
    bool find_space(size_t size, adrs_t& adrs)
    {
        //  Find the first freespace that contains size bytes
        for (auto it = free_.begin(); it != free_.end(); ++it)
        {
            if (it->size >= size)
            {
                adrs = it->start;
                return true;
            }
        }
        return false;
    }

    const std::array<uint8_t, 32768> get_content() const
    {
        std::array<uint8_t, 32768> content;

        // Copy data_[0x8000-0xa000] into content[0x0000-0x2000]
        std::copy(data_.begin() + 0x8000, data_.begin() + 0xA000, content.begin());

        // Copy data_[0x2000-0x8000] into content[0x2000-0x8000]
        std::copy(data_.begin() + 0x2000, data_.begin() + 0x8000, content.begin() + 0x2000);

        return content;
    }
};

// TODO : sort allocation needs :
//->size = > page, adrs
//->adrs = > page, [adrs]
//-> page, adrs = > [page],[adrs]

/// A 512K apple1 rom
class rom512
{
    std::array<rombank32, BankCount> banks_;

   public:
    void reserve(const pagedadrs_t& pagedadrs, size_t size)
    {
        // get the bank
        auto& bank = banks_[pagedadrs.get_page()];
        auto adrs = pagedadrs.get_address();

        std::clog << "Reserving " << size << " bytes at " << pagedadrs.to_string() << std::endl;

        // add the data
        bank.reserve(adrs, size);
    }

    void store_unchecked(const std::vector<uint8_t>& data, const pagedadrs_t& pagedadrs)
    {
        // get the bank
        auto& bank = banks_[pagedadrs.get_page()];
        auto adrs = pagedadrs.get_address();

        std::clog << "Storing unchecked " << data.size() << " bytes at " << pagedadrs.to_string()
                  << std::endl;

        // add the data
        bank.store_unchecked(data, adrs);
    }

    void store(const std::vector<uint8_t>& data, const pagedadrs_t& pagedadrs)
    {
        // get the bank
        auto& bank = banks_[pagedadrs.get_page()];
        auto adrs = pagedadrs.get_address();

        std::clog << "Storing " << data.size() << " bytes at " << pagedadrs.to_string()
                  << std::endl;

        // add the data
        bank.store(data, adrs);
    }

    //  Find the first space in the first bank that has size bytes available
    //  at address adrs
    bool find_space(size_t size, adrs_t adrs, pagedadrs_t& pagedadrs)
    {
        for (size_t i = 0; i < BankCount; ++i)
            if (banks_[i].check_free(adrs, size))
            {
                pagedadrs = pagedadrs_t(i, adrs);
                return true;
            }
        return false;
    }

    //  Find the first space in the first bank that has size bytes available
    bool find_space(size_t size, pagedadrs_t& pagedadrs)
    {
        adrs_t adrs(0);
        for (size_t i = 0; i < BankCount; ++i)
            if (banks_[i].find_space(size, adrs))
            {
                pagedadrs = pagedadrs_t(i, adrs);
                return true;
            }
        return false;
    }

    const std::array<uint8_t, 32768 * BankCount> get_content() const
    {
        std::array<uint8_t, 32768 * BankCount> content;

        // Concatenate all the banks_[]->get_content() into result
        for (size_t i = 0; i < BankCount; ++i)
        {
            auto bank_content = banks_[i].get_content();
            std::copy(bank_content.begin(), bank_content.end(), content.begin() + i * 32768);
        }

        return content;
    }
};
