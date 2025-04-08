#pragma once

#include <memory>

#include "adrs.hpp"
#include "emiter.hpp"
#include "pagedadrs.hpp"

class action
{
   public:
    virtual void doit(emiter &emiter) const = 0;
    virtual ~action() {}
};

class execaction : public action
{
    pagedadrs_t code_;

   public:
    execaction(pagedadrs_t code) : code_(code) {}

    virtual void doit(emiter &emiter) const { emiter.JMP(code_); }
};

class loadaction : public action
{
    adrs_t adrs_;
    pagedadrs_t pagedadrs_;
    size_t len_;

   public:
    loadaction(adrs_t adrs, pagedadrs_t pagedadrs, size_t len)
        : adrs_(adrs), pagedadrs_(pagedadrs), len_(len)
    {
    }

    virtual void doit(emiter &emiter) const
    {
        std::clog << "; Copy " << len_ << " bytes from " << pagedadrs_.to_string() << " to "
                  << adrs_.to_string() << std::endl;
        emiter.CALL((adrs_t)0x2018);
        emiter.WORD((uint16_t)adrs_);
        emiter.WORD((uint16_t)pagedadrs_.get_address());
        emiter.BYTE(pagedadrs_.get_page());
        emiter.WORD(len_);
    }
};