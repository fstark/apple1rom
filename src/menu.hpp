#pragma once

#include <iostream>
#include <memory>  // Ensure this is included for std::make_shared
#include <string>  // Ensure this is included for std::string
#include <vector>

#include "adrs.hpp"
#include "asm.hpp"
#include "pagedadrs.hpp"

class emiter
{
    adrs_t start_;

    std::vector<uint8_t> code_;
    void emit(uint8_t data) { code_.push_back(data); }

   public:
    emiter(adrs_t adrs) : start_{adrs} {}

    const std::vector<uint8_t> &code() const { return code_; }

    adrs_t current() const { return start_ + code_.size(); }

    void BYTE(uint8_t data)
    {
        std::clog << "BYTE " << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                  << (int)data << std::endl;
        emit(data);
    }

    void WORD(uint16_t data)
    {
        std::clog << "WORD " << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
                  << (int)data << std::endl;
        emit(data & 0xff);
        emit(data >> 8);
    }

    void ADRS(adrs_t adrs)
    {
        BYTE(0);  // #### REMOVE ME
        WORD((uint16_t)adrs);
    }

    void STRZ(const char *p)
    {
        do
        {
            BYTE(*p++);
        } while (p[-1]);
    }

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
        JMP(LONGJUMP0);
    }
    void LDA(uint8_t data)
    {
        std::clog << "LDA #" << std::hex << (int)data << std::endl;
        emit(0xa9);
        emit(data);
    }
    void STA(uint8_t data)
    {
        std::clog << "STA #" << std::hex << (int)data << std::endl;
        emit(0x85);
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

    void LOADZP(uint8_t adrsZP, adrs_t adrs)
    {
        // std::clog << "LOADZP " << adrsZP << " = " << adrs << " " << std::endl;

        LDA(adrs.get_low());
        STA(adrsZP);  // should be ZP opcode
        LDA(adrs.get_high());
        STA(adrsZP + 1);
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

class menu_item
{
    std::string name_;
    bool adrs_resolved_;
    adrs_t adrs_;

   public:
    menu_item(const std::string &name) : name_{name}, adrs_resolved_{false}, adrs_{0} {}

    virtual ~menu_item() {}

    const std::string &name() const { return name_; }
    const char key() const { return short_name()[0]; }
    const pagedadrs_t pagedadrs() const { return pagedadrs_t(0, adrs_t(0)); }
    adrs_t adrs() const { return adrs_; }

    const std::vector<std::string> get_path() const
    {
        std::vector<std::string> result;
        size_t start = 0;
        size_t end = name_.find('/');
        while (end != std::string::npos)
        {
            result.push_back(name_.substr(start, end - start));
            start = end + 1;
            end = name_.find('/', start);
        }
        result.push_back(name_.substr(start));
        return result;
    }

    const std::string short_name() const
    {
        auto path = get_path();
        if (path.size() > 1)
            return path.back();
        else
            return name_;
    }

    virtual void emit(emiter &e) = 0;

    void resolve_adrs(const emiter &e)
    {
        if (adrs_resolved_ && adrs_ != e.current())
            if (adrs_resolved_ && adrs_ != e.current())
            {
                std::clog << (uint16_t)adrs_ << " != " << e.current().to_string() << std::endl;
                throw std::runtime_error("Address already resolved");
            }
        adrs_ = e.current();
        adrs_resolved_ = true;
    }

    /// Emit a menu entry (that jumps to the adrs)
    void emit_entry(emiter &e)
    {
        std::clog << "; This menu entry defines [" << name() << "]" << std::endl;
        e.BYTE(toupper(key()));
        e.ADRS(adrs_);
        e.STRZ(short_name().c_str());
        std::clog << "; (end of entry)" << std::endl;
    }
};

class menu_action : public menu_item
{
    std::vector<std::shared_ptr<action>> actions_;

   public:
    menu_action(const std::string &name) : menu_item{name} {}

    void add_action(std::shared_ptr<action> a)
    {  // Add action
        actions_.push_back(a);
    }

    const std::vector<std::shared_ptr<action>> &actions() const { return actions_; }
    // Generates the assembly code for the actions
    void emit(emiter &e)
    {
        std::clog << "; This code will launch [" << name() << "]" << std::endl;
        resolve_adrs(e);
        for (const auto &action : actions_)
        {
            action->doit(e);
        }
        std::clog << "; (end of code)" << std::endl;
    }
};

/* A menu is represented as the following bytes:
    1 byte: key (firs char of menu name)
    2-4 bytes: pagedadrs_t of the menu
    string: menu name, 0 terminated

    if key == 0, it is the end of the menu
*/
class menu : public menu_item
{
    std::vector<std::shared_ptr<menu>> submenus_;
    std::vector<std::shared_ptr<menu_action>> items_;

    adrs_t menudef_adrs_{0};

   public:
    menu(const std::string &name) : menu_item(name) {}
    void add_menu(std::shared_ptr<menu> submenu) { submenus_.push_back(submenu); }
    void add_item(std::shared_ptr<menu_action> item) { items_.push_back(item); }
    void add_item_path(std::shared_ptr<menu_action> item);

    std::vector<uint8_t> menudef() const
    {
        std::vector<uint8_t> result;
        for (const auto &item : items_)
        {
            result.push_back(item->key());
            auto pagedadrs = item->pagedadrs();
            result.push_back(pagedadrs.get_page());
            result.push_back(pagedadrs.get_address().get_low());
            result.push_back(pagedadrs.get_address().get_high());
            for (const auto &c : item->name()) result.push_back(c);
            result.push_back(0);
        }
        result.push_back(0);
        return result;
    }

    bool is_item_present(const std::string &name) const
    {
        for (const auto &item : items_)
        {
            if (item->name() == name)
            {
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<menu> get_submenu(const std::string &name) const
    {
        for (const auto &menu : submenus_)
        {
            if (menu->name() == name)
            {
                return menu;
            }
        }
        return nullptr;
    }

    void emit(emiter &e);

    void dump()
    {
        std::clog << "Menu [" << name() << "]" << std::endl;
        std::clog << "Submenus:" << std::endl;
        for (const auto &submenu : submenus_)
        {
            std::clog << "  " << submenu->name() << std::endl;
        }
        std::clog << "Items:" << std::endl;
        for (const auto &item : items_)
        {
            std::clog << "  " << item->name() << std::endl;
        }

        std::clog << "Dump of Submenus" << std::endl;
        for (const auto &submenu : submenus_)
        {
            submenu->dump();
        }
    }
};
