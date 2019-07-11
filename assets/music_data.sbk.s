.include "macros.inc"

.section .data

.macro seq_table name
.word (\name - gMusicData), (\name\()_end - \name)
.endm

glabel gMusicData # region 745F80 (US: 7B0860)
.hword 3 # current segment
.hword (music_sequence_table_end - music_sequence_table) / 8 # number of entries

music_sequence_table:
seq_table seq_00
seq_table seq_01
seq_table seq_02
seq_table seq_03
seq_table seq_04
seq_table seq_05
seq_table seq_06
seq_table seq_07
seq_table seq_08
seq_table seq_09
seq_table seq_0A
seq_table seq_0B
seq_table seq_0C
seq_table seq_0D
seq_table seq_0E
seq_table seq_0F
seq_table seq_10
seq_table seq_11
seq_table seq_12
seq_table seq_13
seq_table seq_14
seq_table seq_15
seq_table seq_16
seq_table seq_17
seq_table seq_18
seq_table seq_19
seq_table seq_1A
seq_table seq_1B
seq_table seq_1C
seq_table seq_1D
seq_table seq_1E
seq_table seq_1F
seq_table seq_20
seq_table seq_21
.ifdef VERSION_US
seq_table seq_22
.endif
music_sequence_table_end:

.ifdef VERSION_JP
.word 0, 0, 0
.else
.word 0
.endif

.macro sequence name
\name:
.ifdef VERSION_JP
.incbin "assets/music/jp/\name\().m64"
.endif
.ifdef VERSION_US
.incbin "assets/music/us/\name\().m64"
.endif
\name\()_end:
.endm

sequence seq_00
sequence seq_01
sequence seq_02
sequence seq_03
sequence seq_04
sequence seq_05
sequence seq_06
sequence seq_07
sequence seq_08
sequence seq_09
sequence seq_0A
sequence seq_0B
sequence seq_0C
sequence seq_0D
sequence seq_0E
sequence seq_0F
sequence seq_10
sequence seq_11
sequence seq_12
sequence seq_13
sequence seq_14
sequence seq_15
sequence seq_16
sequence seq_17
sequence seq_18
sequence seq_19
sequence seq_1A
sequence seq_1B
sequence seq_1C
sequence seq_1D
sequence seq_1E
sequence seq_1F
sequence seq_20
sequence seq_21
.ifdef VERSION_US
sequence seq_22
.endif

.ifdef VERSION_JP # for some reason only the JP version has this padding.
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
.endif

.macro instruments name
.hword (\name - gInstrumentSets)
.endm

glabel gInstrumentSets
instruments instrument_set_00
instruments instrument_set_01
instruments instrument_set_02
instruments instrument_set_03
instruments instrument_set_04
instruments instrument_set_05
instruments instrument_set_06
instruments instrument_set_07
instruments instrument_set_08
instruments instrument_set_09
instruments instrument_set_0A
instruments instrument_set_0B
instruments instrument_set_0C
instruments instrument_set_0D
instruments instrument_set_0E
instruments instrument_set_0F
instruments instrument_set_10
instruments instrument_set_11
instruments instrument_set_12
instruments instrument_set_13
instruments instrument_set_14
instruments instrument_set_15
instruments instrument_set_16
instruments instrument_set_17
instruments instrument_set_18
instruments instrument_set_19
instruments instrument_set_1A
instruments instrument_set_1B
instruments instrument_set_1C
instruments instrument_set_1D
instruments instrument_set_1E
instruments instrument_set_1F
instruments instrument_set_20
instruments instrument_set_21
.ifdef VERSION_US
instruments instrument_set_22
.endif

instrument_set_00:
.byte 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
instrument_set_01:
.byte 0x01, 0x22
instrument_set_02:
.byte 0x01, 0x11
instrument_set_03:
.byte 0x01, 0x22
instrument_set_04:
.byte 0x01, 0x0E
instrument_set_05:
.byte 0x01, 0x13
instrument_set_06:
.byte 0x01, 0x0F
instrument_set_07:
.byte 0x01, 0x12
instrument_set_08:
.byte 0x01, 0x0B
instrument_set_09:
.byte 0x01, 0x0D
instrument_set_0A:
.byte 0x02, 0x21, 0x10
instrument_set_0B:
.byte 0x01, 0x14
instrument_set_0C:
.byte 0x01, 0x15
instrument_set_0D:
.byte 0x01, 0x16
instrument_set_0E:
.byte 0x01, 0x17
instrument_set_0F:
.byte 0x01, 0x18
instrument_set_10:
.byte 0x01, 0x12
instrument_set_11:
.byte 0x01, 0x19
instrument_set_12:
.byte 0x01, 0x1F
instrument_set_13:
.byte 0x01, 0x21
instrument_set_14:
.byte 0x01, 0x1A
instrument_set_15:
.byte 0x01, 0x0E
instrument_set_16:
.byte 0x01, 0x1B
instrument_set_17:
.byte 0x01, 0x1A
instrument_set_18:
.byte 0x01, 0x1C
instrument_set_19:
.byte 0x01, 0x1D
instrument_set_1A:
.byte 0x01, 0x25
instrument_set_1B:
.byte 0x01, 0x14
instrument_set_1C:
.byte 0x01, 0x20
instrument_set_1D:
.byte 0x01, 0x1E
instrument_set_1E:
.byte 0x01, 0x1B
instrument_set_1F:
.byte 0x01, 0x1A
instrument_set_20:
.byte 0x01, 0x23
instrument_set_21:
.byte 0x01, 0x24
.ifdef VERSION_US
instrument_set_22:
.byte 0x01, 0x1B
.endif
instrument_sets_end:
