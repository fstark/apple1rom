// This header file defines the `menu_item` class, serving as a base class for menu-related items.
// It provides common functionality such as address resolution, emitting menu entries, and
// retrieving item paths.
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
    // Initializes a `menu_item` with a name.
    menu_item(const std::string &name) : name_{name}, adrs_resolved_{false}, adrs_{0} {}

    // Virtual destructor for proper cleanup in derived classes.
    virtual ~menu_item() {}

    // Retrieves the name of the menu item.
    const std::string &name() const { return name_; }
    // Retrieves the key associated with the menu item.
    const char key() const { return short_name()[0]; }
    // Retrieves the paged address of the menu item.
    const pagedadrs_t pagedadrs() const { return pagedadrs_t(0, adrs_t(0)); }
    // Resolves the address of the menu item.
    adrs_t adrs() const { return adrs_; }

    const std::vector<std::string> get_path() const;
    const std::string short_name() const;

    // Emits assembly code for the menu item.
    virtual void emit(emiter &e) = 0;

    void resolve_adrs(const emiter &e);

    // Emits a menu entry that jumps to the address.
    void emit_entry(emiter &e);
};
