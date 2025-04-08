#include "menu.hpp"

void menu::add_item_path(std::shared_ptr<menu_action> item)
{
    auto path = item->get_path();
    menu *current = this;

    // all the menus
    while (path.size() != 1)
    {
        const std::string name = path[0];
        std::clog << name << "\n";
        auto submenu = get_submenu(name);
        if (!submenu)
        {
            std::clog << "Creating a submenu for [" << name << "]" << std::endl;
            submenu = std::make_shared<menu>(name);
            current->add_menu(submenu);
        }
        current = submenu.get();
        path.erase(path.begin());  // Remove the first element
    }

    // Add the item to the current menu
    current->add_item(item);
}

void menu::emit(emiter &e)
{
    std::clog << ">>>> Emitting menu execution code for [" << name() << "]" << std::endl;

    // Load adrs into MENU ($00-$01)
    resolve_adrs(e);
    e.LOADZP(0x0000, menudef_adrs_);
    e.JMP(DISPLAY_MENU);

    std::clog << "<<<< DONE" << std::endl;

    std::clog << ">>>> Emitting submenus of [" << name() << "]" << std::endl;
    for (auto &m : submenus_)
    {
        m->emit(e);
    }
    std::clog << "<<<< Done" << std::endl;

    std::clog << ">>>> Emitting items of [" << name() << "]" << std::endl;
    for (auto &mi : items_)
    {
        mi->emit(e);
    }
    std::clog << "<<<< Done" << std::endl;

    std::clog << ">>>> Emitting menu definition" << std::endl;

    menudef_adrs_ = e.current();

    for (auto &m : submenus_) m->emit_entry(e);
    for (auto &mi : items_) mi->emit_entry(e);
    e.BYTE(0x00);  // End-of-menu

    std::clog << "<<<< Done" << std::endl;
}

std::vector<uint8_t> menu::menudef() const
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

bool menu::is_item_present(const std::string &name) const
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

std::shared_ptr<menu> menu::get_submenu(const std::string &name) const
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

void menu::dump()
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
