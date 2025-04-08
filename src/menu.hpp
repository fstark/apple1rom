#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "menu_item.hpp"

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

    std::vector<uint8_t> menudef() const;
    bool is_item_present(const std::string &name) const;
    std::shared_ptr<menu> get_submenu(const std::string &name) const;
    void emit(emiter &e);
    void dump();
};
