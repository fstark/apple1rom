; Assembly code for the 512K rom

; At 00:2100 code that displays a menu

; BANK = $00
; ADRS0 = $01 ; (two bytes)
; ADRS1 = $03 ; (two bytes)
; LEN = $05 ; (two bytes)

MENU = $00
PTR = $02
KEY = $04

BANK = $06
EXEC = $07 ; (two bytes)


DISPLAYMENU = $2100
ROM0BOOT = $2200

ECHO = $FFEF
KBD = $D010
KBDCR = $D011


; Displays the menu that is specifed in the ZP:MENU
* = DISPLAYMENU
.(
;    LDA #<MENUDATA
;    STA MENU
;    LDA #>MENUDATA
;    STA MENU+1
    JSR DISPLAYALL
LOOP:
    JSR KEYALL
    JMP LOOP
.)

; Display all the menu entries
DISPLAYALL:
.(
    LDA MENU
    STA PTR
    LDA MENU+1
    STA PTR+1

    LDA #$0d
    JSR ECHO

    LDY #0
NEXTENTRY:
    LDA (PTR),Y         ; Last menu entry?
    BEQ DONE            ; Done
    JSR DISPLAY1        ; Display menu entry
    JMP NEXTENTRY

DONE:
    LDA #$0d
    JSR ECHO

    RTS
.)

; Echo a character to the screen
; and manage keyboard interadction if a key is pressed
ECHOYIELD:
    JSR ECHO
    JMP KEYALL

; If a key is pressed, manage it
KEYALL:
.(
    LDA KBDCR
    BPL DONE
    LDA KBD             ;   Key pressed
    AND #$7F            ;   Last 7 bits
    STA KEY

    LDA PTR
    PHA
    LDA PTR+1
    PHA

    LDA MENU
    STA PTR
    LDA MENU+1
    STA PTR+1

    JSR CHECKKEYS

    PLA
    STA PTR+1
    PLA
    STA PTR

DONE:
    RTS
.)

; Check the content of KEY with all the keys of the current menu
; Executes the corresponding code if found
CHECKKEYS:
.(
    LDY #0
NEXTENTRY:
    LDA (PTR),Y
    BEQ DONE
    CMP KEY
    BEQ EXECUTE

    ; LDA (PTR),Y
    ; JSR ECHO
    ; LDA KEY
    ; JSR ECHO
    ; LDA #' '
    ; JSR ECHO

    JSR INCPTR      ; bank
    JSR INCPTR      ; addressL
    JSR INCPTR      ; addressH
LOOP:
    JSR INCPTR
    LDA (PTR),Y
    BNE LOOP
    JSR INCPTR
    JMP NEXTENTRY
DONE:
    JMP INCPTR
.)


EXECUTE:
.(
    JSR INCPTR      ; bank
    LDA (PTR),Y
    STA BANK
    JSR INCPTR      ; addressL
    LDA (PTR),Y
    STA EXEC
    JSR INCPTR      ; addressH
    LDA (PTR),Y
    STA EXEC+1

    JMP (EXEC)
.)

AAA:
.(
    LDA #'A'
    JSR ECHO
    JMP AAA
.)

BBB:
.(
    LDA #'B'
    JSR ECHO
    JMP BBB
.)

DISPLAY1:
.(
    LDA (PTR),Y
    JSR ECHOYIELD
    LDA #')'
    JSR ECHOYIELD
    LDX #18
    JSR INCPTR
    JSR INCPTR
    JSR INCPTR
LOOP:
    JSR INCPTR
    LDA (PTR),Y
    BEQ DONE
    JSR ECHOYIELD
    DEX
    JMP LOOP
DONE:
    JSR INCPTR
SPACELOOP:
    LDA #' '
    JSR ECHOYIELD
    DEX
    BNE SPACELOOP
 
    RTS
.)

INCPTR:
.(
    INC PTR
    BNE NOINC
    INC PTR+1
NOINC:
    RTS
.)

; MENU structure
; KEY : 1 byte
; BANK: 1 byte
; ADRS: 2 bytes
; TEXT: n bytes
; 0x00

; 0x00 for end of menu

.dsb ROM0BOOT-*,$ff
;    .byt 'D', $00, <AAA, >AAA, "Demos", $00
;    .byt 'G', $00, <BBB, >BBB, "Games", $00
;    .byt 'L', $00, <AAA, >AAA, "Languages", $00
;    .byt 'U', $00, <AAA, >AAA, "Utilities", $00
;    .byt 'M', $00, <AAA, >AAA, "Misc", $00
;    .byt $00
