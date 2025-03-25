; Assembly code for the 512K rom, copied to all banks

; At 2000: code that boots the menu

BANK = $00
ADRS0 = $01 ; (two bytes)
ADRS1 = $03 ; (two bytes)
LEN = $05 ; (two bytes)

ORG $2000

BOOT:
    JSR SWITCH0
    JMP MENU ; unsure where menu is supposed to be

SWITCH0:
    LDA #0

; Switch to bank A
SWITCH:
.(
    TAX
    BIT ($2000),X
    RET
).

; COPY
COPY:
.(
    LDA BANK
    JSR SWITCH
    LDX #0
LOOP:
    LDA LEN
    ORA LEN+1
    BEQ DONE
    LDA (ADRS1),X
    STA (ADRS0),X
    ; decrement 16 bit value in LEN, LEN+1
    LDA LEN
    SEC
    SBC #1
    STA LEN
    LDA LEN+1
    SBC #0
    STA LEN+1
    JMP LOOP
DONE:
    RTS
).
