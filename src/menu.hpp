// This header file defines the `menu` class, representing a hierarchical menu structure.
// It allows adding submenus and items, generating menu definitions, and emitting assembly code.
#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "menu_action.hpp"  // Added include
#include "menu_item.hpp"

class menu : public menu_item
{
    std::vector<std::shared_ptr<menu>> submenus_;
    std::vector<std::shared_ptr<menu_action>> items_;

    adrs_t menudef_adrs_{0};

   public:
    // Initializes a `menu` with a name.
    menu(const std::string &name) : menu_item(name) {}

    // Adds a submenu to the menu.
    void add_menu(std::shared_ptr<menu> submenu) { submenus_.push_back(submenu); }

    // Adds an item to the menu.
    void add_item(std::shared_ptr<menu_action> item) { items_.push_back(item); }

    // Adds an item to the menu by path.
    void add_item_path(std::shared_ptr<menu_action> item);

    // Generates the menu definition as a vector of bytes.
    std::vector<uint8_t> menudef() const;

    // Checks if an item is present in the menu by name.
    bool is_item_present(const std::string &name) const;

    // Retrieves a submenu by name.
    std::shared_ptr<menu> get_submenu(const std::string &name) const;

    // Emits assembly code for the menu.
    void emit(emiter &e);

    // Dumps the menu structure for debugging purposes.
    void dump();
};
