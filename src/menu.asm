; Assembly code for the 512K rom, copied to all banks

; At 2000: code that boots the menu

BANK = $00
ADRS0 = $01 ; (two bytes)
ADRS1 = $03 ; (two bytes)
LEN = $05 ; (two bytes)

MENU = $2100

    * = MENU
