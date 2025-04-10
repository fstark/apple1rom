#include "action.hpp"

#include <iostream>

void execaction::doit(emiter &emiter) const { emiter.JMP(code_); }

void loadaction::doit(emiter &emiter) const
{
    std::clog << "; Copy " << len_ << " bytes from " << pagedadrs_.to_string() << " to "
              << adrs_.to_string() << std::endl;
    emiter.CALL((adrs_t)0x2018);
    emiter.WORD((uint16_t)adrs_);
    emiter.WORD((uint16_t)pagedadrs_.get_address());
    emiter.BYTE(pagedadrs_.get_page());
    emiter.WORD(len_);
}
