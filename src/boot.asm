; Assembly code for the 512K rom, copied to all banks

; At 2000: code that boots the menu

ADRSTO = $00 ; (two bytes)
ADRSFROM = $02 ; (two bytes)
BANKFROM = $04
LEN = $05 ; (two bytes)

ARGS = $07

BOOT = $2000
SWITCH0 = $2008
LONGJMP = $2010
COPY = $2018

ROM0CODE = $2100
DISPLAYMENU = $2200

PANIC = $2020       ; ROM should be filled with $20 by default, making spurious bank switches
                    ; perfoming a JSR $2020
BANK = $9FFF        ; $9FFF contains the current bank number
ECHO = $FFEF
PRBYTE = $FFDC


    * = BOOT
; 2000 : The BOOT code
    JSR SWITCH0
    JMP DISPLAYMENU

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
    JMP COPY_IMPL

; Handler of panic
; Displays the bank number and the address we came from (+2)
    .dsb PANIC-*,$ff
.(
    LDA #$0d
    JSR ECHO
    LDA BANK
    JSR PRBYTE
    LDA #':'
    JSR ECHO
    PLA
    TAX
    PLA
    JSR PRBYTE
    TXA
    JSR PRBYTE
    LDA #$0d
    JSR ECHO
LOOP:
    LDA BANK
    JSR PRBYTE
    LDA #' '
    JSR ECHO
    JMP LOOP
.)

COPY_IMPL:
.(
    PLA
    STA ARGS
    PLA
    STA ARGS+1
    LDY #1

    LDA (ARGS),Y
    STA ADRSTO
    INY
    LDA (ARGS),Y
    STA ADRSTO+1
    INY

    LDA (ARGS),Y
    STA ADRSFROM
    INY

    LDA (ARGS),Y
    STA ADRSFROM+1
    INY

    LDA (ARGS),Y
    STA BANKFROM
    INY

    LDA (ARGS),Y
    STA LEN
    INY

    LDA (ARGS),Y
    STA LEN+1
    INY

    LDA BANKFROM
    JSR SWITCH
    LDY #0

LOOP:
        ; If LEN==0 then we are done
    LDA LEN
    ORA LEN+1
    BEQ DONE

        ; Copy one
    LDA (ADRSFROM),Y
    STA (ADRSTO),Y

        ; LEN--
    LDA LEN+0       ;Test if the LSB is zero
    BNE SKIP0       ;If it isn't we can skip the next instruction
    DEC LEN+1       ;Decrement the MSB when the LSB will underflow
SKIP0:
    DEC LEN+0

        ; ADRSFROM++
    INC ADRSFROM
    BNE SKIP1
    INC ADRSFROM+1
SKIP1:

        ; ADRSTO++
    INC ADRSTO
    BNE SKIP2 ; can jump to lop directly to save some cycles
    INC ADRSTO+1
SKIP2:

    JMP LOOP

DONE:

        ; Increment return address by 7
    CLC
    LDA ARGS
    ADC #7
    STA ARGS
    BCC SKIP3
    INC ARGS+1
SKIP3:

        ; Push retrun address and RTS
    LDA ARGS+1
    PHA
    LDA ARGS
    PHA
    RTS
.)

; Checking we don't go over the rom0 code
    .dsb ROM0CODE-*,$ff
