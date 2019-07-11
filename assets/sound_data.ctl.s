.include "macros.inc"

.section .data

.macro adsrtable name
.word (\name - gSoundDataADSR), (\name\()_end - \name)
.endm

.macro adsr name
\name:
.ifdef VERSION_JP
.incbin "assets/sound/adsr/jp/\name\().bin"
.endif
.ifdef VERSION_US
.incbin "assets/sound/adsr/us/\name\().bin"
.endif
\name\()_end:
.endm

glabel gSoundDataADSR # US: 57B720
.hword 1 # current sequence
.hword (adsr_entry_end - adsr_entry_start) / 8 # number of entries

adsr_entry_start:
adsrtable adsr_00
adsrtable adsr_01
adsrtable adsr_02
adsrtable adsr_03
adsrtable adsr_04
adsrtable adsr_05
adsrtable adsr_06
adsrtable adsr_07
adsrtable adsr_08
adsrtable adsr_09
adsrtable adsr_0A
adsrtable adsr_0B
adsrtable adsr_0C
adsrtable adsr_0D
adsrtable adsr_0E
adsrtable adsr_0F
adsrtable adsr_10
adsrtable adsr_11
adsrtable adsr_12
adsrtable adsr_13
adsrtable adsr_14
adsrtable adsr_15
adsrtable adsr_16
adsrtable adsr_17
adsrtable adsr_18
adsrtable adsr_19
adsrtable adsr_1A
adsrtable adsr_1B
adsrtable adsr_1C
adsrtable adsr_1D
adsrtable adsr_1E
adsrtable adsr_1F
adsrtable adsr_20
adsrtable adsr_21
adsrtable adsr_22
adsrtable adsr_23
adsrtable adsr_24
adsrtable adsr_25
adsr_entry_end:

.word 0, 0, 0 # you cant use an align 4, 0x00 here or else the table calculation above will not work.

adsr adsr_00
adsr adsr_01
adsr adsr_02
adsr adsr_03
adsr adsr_04
adsr adsr_05
adsr adsr_06
adsr adsr_07
adsr adsr_08
adsr adsr_09
adsr adsr_0A
adsr adsr_0B
adsr adsr_0C
adsr adsr_0D
adsr adsr_0E
adsr adsr_0F
adsr adsr_10
adsr adsr_11
adsr adsr_12
adsr adsr_13
adsr adsr_14
adsr adsr_15
adsr adsr_16
adsr adsr_17
adsr adsr_18
adsr adsr_19
adsr adsr_1A
adsr adsr_1B
adsr adsr_1C
adsr adsr_1D
adsr adsr_1E
adsr adsr_1F
adsr adsr_20
adsr adsr_21
adsr adsr_22
adsr adsr_23
adsr adsr_24
adsr adsr_25

.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
.ifdef VERSION_US
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
.endif
