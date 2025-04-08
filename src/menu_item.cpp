#include "menu_item.hpp"

void menu_item::resolve_adrs(const emiter &e)
{
    if (adrs_resolved_ && adrs_ != e.current())
    {
        std::clog << (uint16_t)adrs_ << " != " << e.current().to_string() << std::endl;
        throw std::runtime_error("Address already resolved");
    }
    adrs_ = e.current();
    adrs_resolved_ = true;
}

void menu_item::emit_entry(emiter &e)
{
    std::clog << "; This menu entry defines [" << name() << "]" << std::endl;
    e.BYTE(toupper(key()));
    e.ADRS(adrs_);
    e.STRZ(short_name().c_str());
    std::clog << "; (end of entry)" << std::endl;
}

const std::vector<std::string> menu_item::get_path() const
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

const std::string menu_item::short_name() const
{
    auto path = get_path();
    if (path.size() > 1)
        return path.back();
    else
        return name_;
}

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
