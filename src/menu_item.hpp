#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "action.hpp"
#include "emiter.hpp"

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

    const std::vector<std::string> get_path() const;
    const std::string short_name() const;

    virtual void emit(emiter &e) = 0;

    void resolve_adrs(const emiter &e);

    /// Emit a menu entry (that jumps to the adrs)
    void emit_entry(emiter &e);
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
    void emit(emiter &e);
};
