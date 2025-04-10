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

    virtual void doit(emiter &emiter) const override;
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

    virtual void doit(emiter &emiter) const override;
};