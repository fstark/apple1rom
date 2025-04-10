#include "menu_action.hpp"

void menu_action::emit(emiter &e)
{
    std::clog << "; This code will launch [" << name() << "]" << std::endl;
    resolve_adrs(e);
    for (const auto &action : actions_)
    {
        action->doit(e);
    }
    std::clog << "; (end of code)" << std::endl;
}
