.include "macros.inc"

.section .data

.macro soundtable name
.word (\name - gSoundDataRaw), (\name\()_end - \name)
.endm

.macro sound bank, name
.ifdef VERSION_JP
.align 4, 0x00
.incbin "assets/sound/jp/\bank/\name\.bin"
.endif
.ifdef VERSION_US
.align 4, 0x00
.incbin "assets/sound/us/\bank/\name\.bin"
.endif
.endm

# US: 593560 - 7B0860
# raw sound data area
raw_sound_begin:
glabel gSoundDataRaw # the nlist entries below are part of this area.
.hword 2 # current segment
.hword (sound_table_end - sound_table) / 8 # number of entries (pair of words as described below)

# Each pair of words represents the following:
# 1: Relative offset of the nlist
# 2: Size of the nlist

sound_table:
soundtable nlist_00
soundtable nlist_01
soundtable nlist_02
soundtable nlist_03
soundtable nlist_04_05
soundtable nlist_04_05 
soundtable nlist_06
soundtable nlist_07
soundtable nlist_08
soundtable nlist_09
soundtable nlist_0A
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_extra_1
soundtable nlist_0B_rest
soundtable nlist_extra_2
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_extra_3
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
soundtable nlist_0B_rest
sound_table_end:

.word 0, 0, 0

nlist_00: # US: 5936A0
sound 00, 00
sound 00, 01
sound 00, 02
sound 00, 03
sound 00, 04
sound 00, 05
nlist_00_end:

nlist_01: # US: 59A840
sound 01, 00
sound 01, 01
sound 01, 03
sound 01, 04
sound 01, 05
sound 01, 06
sound 01, 07
sound 01, 08
nlist_01_end:

nlist_02: # US: 59E210
sound 02, 00
sound 02, 01
sound 02, 02
nlist_02_end:

nlist_03: # US: 5A4A20
sound 03, 00
sound 03, 01
sound 03, 02
sound 03, 03
sound 03, 04
sound 03, 05
sound 03, 06
sound 03, 07
sound 03, 08
sound 03, 09
nlist_03_end:

nlist_04_05: # US: 5AFCD0
# nlist 04
sound 04, 00
sound 04, 01
sound 04, 02
sound 04, 03
sound 04, 04
sound 04, 05
sound 04, 06
sound 04, 08
sound 04, 09
sound 04, 0A
sound 04, 0B
sound 04, 0D
sound 04, 0E
sound 04, 0F
# nlist 05
sound 05, 00
sound 05, 01
sound 05, 02
sound 05, 03
# a 0x28D0 sample (04) is listed here in the nlist, but doesnt actually exist
# in the bank. probably cross-bank.
sound 05, 05
sound 05, 06
sound 05, 07
sound 05, 08
sound 05, 09
sound 05, 0A
sound 05, 0B
sound 05, 0C
sound 05, 0D
sound 05, 0E
sound 05, 0F
nlist_04_05_end:

nlist_06: # US: 5E5400
sound 06, 00
sound 06, 01
sound 06, 02
sound 06, 04
sound 06, 05
sound 06, 06
sound 06, 07
sound 06, 08
sound 06, 09
sound 06, 0A
sound 06, 0B
sound 06, 0C
sound 06, 0E
sound 06, 0F
nlist_06_end:

nlist_07: # US: 5F6FA0
sound 07, 00
sound 07, 01
sound 07, 02
sound 07, 03
sound 07, 05
sound 07, 06
sound 07, 07
sound 07, 08
sound 07, 09
sound 07, 0A
sound 07, 0B
sound 07, 0C
sound 07, 0D
.ifdef VERSION_US
sound 07, 0E # chain chomp
.endif
nlist_07_end:

nlist_08: # US: 607770
sound 08, 00
sound 08, 01
sound 08, 02
sound 08, 03
sound 08, 04
sound 08, 05
sound 08, 06
sound 08, 07
sound 08, 08
sound 08, 09
sound 08, 0A
sound 08, 0B
sound 08, 0C
sound 08, 0D
sound 08, 0E
sound 08, 0F
.ifdef VERSION_US
sound 08, 10 # doh
sound 08, 11 # game over
sound 08, 12 # hello
sound 08, 13 # press start to play
sound 08, 14 # boing
sound 08, 15 # nighty nighty zzz ah spaghetti zzz ah ravioli zzz ah mamamia
sound 08, 16 # so long ye' bowser
sound 08, 17 # imma tired
sound 08, 18 # wah hah
sound 08, 19 # yippee
sound 08, 1A # lets-a-go
.endif
nlist_08_end:

nlist_09: # US: 64D350
sound 09, 00p
sound 09, 00s
sound 09, 01
sound 09, 03
sound 09, 04
sound 09, 05
sound 09, 06
nlist_09_end:

nlist_0A: # US: 657EA0
sound 0A, 00
sound 0A, 01
sound 0A, 02
sound 0A, 04
sound 0A, 05
sound 0A, 06
sound 0A, 07
sound 0A, 08
sound 0A, 09
sound 0A, 0A
sound 0A, 0B
sound 0A, 0C
sound 0A, 0D
sound 0A, 0E
.ifdef VERSION_US
sound 0A, 0F # dear mario
sound 0A, 10 # mario
sound 0A, 11 # power of the stars
sound 0A, 12 # and its all thanks to you
sound 0A, 13 # thank you mario
sound 0A, 14 # we have to do something special
sound 0A, 15 # lets bake a delicious cake
sound 0A, 16 # for mario
sound 0A, 17 # mario
.endif
nlist_0A_end:

# TODO: Use instrument names
nlist_0B_rest: # US: 6B5B00
sound 0B, 00
sound 0B, 01
sound 0B, 02
sound 0B, 03
sound 0B, 04
sound 0B, 05
sound 0B, 06
sound 0B, 07
sound 0B, 08
sound 0B, 09
sound 0B, 0A
sound 0B, 0B
sound 0B, 0C
sound 0B, 0D
sound 0B, 0E
sound 0B, 0F
sound 0B, 10
sound 0B, 11
sound 0B, 12
sound 0B, 13
sound 0B, 14
sound 0B, 15
sound 0B, 16
sound 0B, 17
sound 0B, 18
sound 0B, 19
sound 0B, 1A
sound 0B, 1B
sound 0B, 1C
sound 0B, 1D
sound 0B, 1E
sound 0B, 1F
sound 0B, 20
sound 0B, 21
sound 0B, 22
sound 0B, 23
sound 0B, 24
sound 0B, 25
sound 0B, 26
sound 0B, 27
sound 0B, 28
sound 0B, 29
sound 0B, 2A
sound 0B, 2B
sound 0B, 2C
sound 0B, 2D
sound 0B, 2E
sound 0B, 2F
sound 0B, 30
sound 0B, 31
sound 0B, 32
sound 0B, 33
sound 0B, 34
sound 0B, 35
sound 0B, 36
sound 0B, 37
sound 0B, 38
sound 0B, 39
sound 0B, 3A
sound 0B, 3B
sound 0B, 3C
sound 0B, 3D
sound 0B, 3E
sound 0B, 3F
sound 0B, 40
sound 0B, 41
sound 0B, 42
sound 0B, 43
sound 0B, 44
sound 0B, 45
sound 0B, 46
sound 0B, 47
sound 0B, 48
nlist_0B_rest_end:

nlist_extra_1: # US: 799DD0
sound extra1 00
nlist_extra_1_end:

nlist_extra_2: # US: 79D850
sound extra2 00
nlist_extra_2_end:

nlist_extra_3: # US: 79F1A0
sound extra3 00
sound extra3 01
sound extra3 02
nlist_extra_3_end:

.word 0, 0, 0, 0
