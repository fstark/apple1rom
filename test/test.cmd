; 8K ram machine
memory reset
memory ram 0000 1FFF

; Loads wozmon
memory rom ../../napple1/machines/WOZMON ff00
;disas ff00 ffff

; Loads a siliconinsider 512K rom
rom512 load ../test/SILICROM512.BIN
rom512 bank 0

; REM sym load rom512/WOZDLE.SYM 2000 9FFF

cpu reset
