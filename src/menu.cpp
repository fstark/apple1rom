#include "menu.hpp"

#include "asm.hpp"

void menu::add_item_path(std::shared_ptr<menu_action> item)
{
    auto path = item->get_path();
    menu *current = this;
    std::string name = "?";

    // all the menus
    while (path.size() != 1)
    {
        name = path[0];
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
