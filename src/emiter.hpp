#pragma once

#include <iostream>
#include <vector>

#include "adrs.hpp"
#include "asm.hpp"
#include "pagedadrs.hpp"

class emiter
{
    adrs_t start_;

    std::vector<uint8_t> code_;
    void emit(uint8_t data) { code_.push_back(data); }

   public:
    emiter(adrs_t adrs) : start_{adrs} {}

    const std::vector<uint8_t> &code() const { return code_; }

    adrs_t current() const { return start_ + code_.size(); }

    void BYTE(uint8_t data)
    {
        std::clog << "BYTE " << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                  << (int)data << std::endl;
        emit(data);
    }

    void WORD(uint16_t data)
    {
        std::clog << "WORD " << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
                  << (int)data << std::endl;
        emit(data & 0xff);
        emit(data >> 8);
    }

    void ADRS(adrs_t adrs)
    {
        BYTE(0);  // #### REMOVE ME
        WORD((uint16_t)adrs);
    }

    void STRZ(const char *p)
    {
        do
        {
            BYTE(*p++);
        } while (p[-1]);
    }

    void CALL(adrs_t adrs)
    {
        std::clog << "CALL " << adrs.to_string() << std::endl;
        emit(0x20);
        emit(adrs.get_low());
        emit(adrs.get_high());
    }
    void JMP(adrs_t adrs)
    {
        std::clog << "JMP " << adrs.to_string() << std::endl;
        emit(0x4c);
        emit(adrs.get_low());
        emit(adrs.get_high());
    }
    void JMP(pagedadrs_t adrs)
    {
        PUSH((uint16_t)(adrs.get_address() - 1));  //  'cause RTS adds 1
        PUSH(adrs.get_page());
        JMP(LONGJUMP0);
    }
    void LDA(uint8_t data)
    {
        std::clog << "LDA #" << std::hex << (int)data << std::endl;
        emit(0xa9);
        emit(data);
    }
    void STA(uint8_t data)
    {
        std::clog << "STA #" << std::hex << (int)data << std::endl;
        emit(0x85);
        emit(data);
    }
    void PUSH(uint8_t data)
    {
        LDA(data);
        std::clog << "PHA" << std::endl;
        emit(0x48);
    }
    void PUSH(uint16_t value)
    {
        PUSH((uint8_t)(value >> 8));
        PUSH((uint8_t)(value & 0xff));
    }

    void LOADZP(uint8_t adrsZP, adrs_t adrs)
    {
        // std::clog << "LOADZP " << adrsZP << " = " << adrs << " " << std::endl;

        LDA(adrs.get_low());
        STA(adrsZP);  // should be ZP opcode
        LDA(adrs.get_high());
        STA(adrsZP + 1);
    }
};