; Assembly code for the 512K rom, copied to all banks

; At 2000: code that boots the menu

BANK = $00
ADRS0 = $01 ; (two bytes)
ADRS1 = $03 ; (two bytes)
LEN = $05 ; (two bytes)

BOOT = $2000
SWITCH0 = $2008
LONGJMP = $2010
COPY = $2018

MENU = $2100


    * = BOOT
; 2000 : The BOOT code
    JSR SWITCH0
    JMP MENU ; Menu is 0:2100

    .dsb SWITCH0-*,$ff
; 2008: Switch to bank 0
    LDA #0

; 200A: Switch to bank A
SWITCH:
.(
    TAX
    STA $A000,X
    RTS
.)

    .dsb LONGJMP-*,$ff
; stack = ... HIGH LOW BANK
.(
    PLA
    JSR SWITCH
    RTS
.)

    .dsb COPY-*,$ff
; COPY
; COPY:
; .(
;     LDA BANK
;     JSR SWITCH
;     LDX #0
; LOOP:
;     LDA LEN
;     ORA LEN+1
;     BEQ DONE
;     LDA (ADRS1),X
;     STA (ADRS0),X
;     ; decrement 16 bit value in LEN, LEN+1
;     LDA LEN
;     SEC
;     SBC #1
;     STA LEN
;     LDA LEN+1
;     SBC #0
;     STA LEN+1
;     JMP LOOP
; DONE:
;     RTS
; ).

; Checking we don't go over the menu
    .dsb MENU-*,$ff
