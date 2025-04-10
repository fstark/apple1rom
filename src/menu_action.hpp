// This header file defines the `menu_action` class, representing a menu item that can perform
// multiple actions. It inherits from `menu_item` and provides methods to add actions and generate
// assembly code for them.
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "action.hpp"
#include "emiter.hpp"
#include "menu_item.hpp"

class menu_action : public menu_item
{
    std::vector<std::shared_ptr<action>> actions_;

   public:
    // Initializes a `menu_action` with a name.
    menu_action(const std::string &name) : menu_item{name} {}

    // Adds an action to the menu item.
    void add_action(std::shared_ptr<action> a)
    {  // Add action
        actions_.push_back(a);
    }

    // Retrieves the list of actions associated with the menu item.
    const std::vector<std::shared_ptr<action>> &actions() const { return actions_; }

    // Generates assembly code for the actions.
    void emit(emiter &e);
};
