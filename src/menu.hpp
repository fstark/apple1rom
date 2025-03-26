#pragma once

#include <iostream>

#include "adrs.hpp"
#include "pagedadrs.hpp"

class emiter
{
    std::vector<uint8_t> code_;
    void emit(uint8_t data) { code_.push_back(data); }

   public:
    const std::vector<uint8_t> &code() const { return code_; }

    void CALL(adrs_t adrs)
    {
        std::clog << "CALL " << adrs.to_string() << std::endl;
        emit(0x20);
        emit(adrs.get_low());
        emit(adrs.get_high());
    }
    void JMP(adrs_t adrs)
    {
        std::clog << "JMP " << adrs.to_string() << std::endl;
        emit(0x4c);
        emit(adrs.get_low());
        emit(adrs.get_high());
    }
    void JMP(pagedadrs_t adrs)
    {
        PUSH((uint16_t)(adrs.get_address() - 1));  //  'cause RTS adds 1
        PUSH(adrs.get_page());
        JMP((adrs_t)0x2010);
    }
    void LDA(uint8_t data)
    {
        std::clog << "LDA #" << std::hex << (int)data << std::endl;
        emit(0xa9);
        emit(data);
    }
    void PUSH(uint8_t data)
    {
        LDA(data);
        std::clog << "PHA" << std::endl;
        emit(0x48);
    }
    void PUSH(uint16_t value)
    {
        PUSH((uint8_t)(value >> 8));
        PUSH((uint8_t)(value & 0xff));
    }
};

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

class menu_item
{
    std::string name_;
    std::vector<std::shared_ptr<action>> actions_;

   public:
    // Constructor that accepts a name and an arbitrary number of actions
    template <typename... Actions>
    menu_item(const std::string &name, Actions... acts) : name_(name)
    {
        // Expand the variadic arguments and add them to the actions_ vector
        (actions_.emplace_back(std::make_shared<Actions>(acts)), ...);
    }

    const std::string &name() const { return name_; }
    const std::vector<std::shared_ptr<action>> &actions() const { return actions_; }
};
