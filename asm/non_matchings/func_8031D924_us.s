glabel func_8031D924
/* 0D97B8 8031E7B8 3C048022 */  lui   $a0, %hi(D_80226D80) # $a0, 0x8022
/* 0D97BC 8031E7BC 24846B80 */  addiu $a0, %lo(D_80226D80) # addiu $a0, $a0, 0x6b80
/* 0D97C0 8031E7C0 8C820000 */  lw    $v0, ($a0)
/* 0D97C4 8031E7C4 27BDFFB8 */  addiu $sp, $sp, -0x48
/* 0D97C8 8031E7C8 3C0F8033 */  lui   $t7, %hi(D_80333EF4) # $t7, 0x8033
/* 0D97CC 8031E7CC 244E0001 */  addiu $t6, $v0, 1
/* 0D97D0 8031E7D0 AFBF0014 */  sw    $ra, 0x14($sp)
/* 0D97D4 8031E7D4 AC8E0000 */  sw    $t6, ($a0)
/* 0D97D8 8031E7D8 25EF5004 */  addiu $t7, %lo(D_80333EF4) # addiu $t7, $t7, 0x5004
/* 0D97DC 8031E7DC 8DF80000 */  lw    $t8, ($t7)
/* 0D97E0 8031E7E0 3C017655 */  lui   $at, (0x76557364 >> 16) # lui $at, 0x7655
/* 0D97E4 8031E7E4 34217364 */  ori   $at, (0x76557364 & 0xFFFF) # ori $at, $at, 0x7364
/* 0D97E8 8031E7E8 13010003 */  beq   $t8, $at, .L_U_8031E7F8
/* 0D97EC 8031E7EC 24020003 */   li    $v0, 3
/* 0D97F0 8031E7F0 100000BF */  b     .L_U_8031EAF0
/* 0D97F4 8031E7F4 00001025 */   move  $v0, $zero
.L_U_8031E7F8:
/* 0D97F8 8031E7F8 3C078022 */  lui   $a3, %hi(D_80226D8C) # $a3, 0x8022
/* 0D97FC 8031E7FC 24E76B8C */  addiu $a3, %lo(D_80226D8C) # addiu $a3, $a3, 0x6b8c
/* 0D9800 8031E800 8CED0000 */  lw    $t5, ($a3)
/* 0D9804 8031E804 3C038022 */  lui   $v1, %hi(D_80226D88) # $v1, 0x8022
/* 0D9808 8031E808 24636B88 */  addiu $v1, %lo(D_80226D88) # addiu $v1, $v1, 0x6b88
/* 0D980C 8031E80C 25AE0001 */  addiu $t6, $t5, 1
/* 0D9810 8031E810 01C2001A */  div   $zero, $t6, $v0
/* 0D9814 8031E814 8C790000 */  lw    $t9, ($v1)
/* 0D9818 8031E818 0000C010 */  mfhi  $t8
/* 0D981C 8031E81C 01C07825 */  move  $t7, $t6
/* 0D9820 8031E820 3B2C0001 */  xori  $t4, $t9, 1
/* 0D9824 8031E824 AC6C0000 */  sw    $t4, ($v1)
/* 0D9828 8031E828 270C0001 */  addiu $t4, $t8, 1
/* 0D982C 8031E82C 0182001A */  div   $zero, $t4, $v0
/* 0D9830 8031E830 ACEE0000 */  sw    $t6, ($a3)
/* 0D9834 8031E834 14400002 */  bnez  $v0, .L_U_8031E840
/* 0D9838 8031E838 00000000 */   nop   
/* 0D983C 8031E83C 0007000D */  break 7
.L_U_8031E840:
/* 0D9840 8031E840 2401FFFF */  li    $at, -1
/* 0D9844 8031E844 14410004 */  bne   $v0, $at, .L_U_8031E858
/* 0D9848 8031E848 3C018000 */   lui   $at, 0x8000
/* 0D984C 8031E84C 15E10002 */  bne   $t7, $at, .L_U_8031E858
/* 0D9850 8031E850 00000000 */   nop   
/* 0D9854 8031E854 0006000D */  break 6
.L_U_8031E858:
/* 0D9858 8031E858 00003010 */  mfhi  $a2
/* 0D985C 8031E85C ACF80000 */  sw    $t8, ($a3)
/* 0D9860 8031E860 0300C825 */  move  $t9, $t8
/* 0D9864 8031E864 14400002 */  bnez  $v0, .L_U_8031E870
/* 0D9868 8031E868 00000000 */   nop   
/* 0D986C 8031E86C 0007000D */  break 7
.L_U_8031E870:
/* 0D9870 8031E870 2401FFFF */  li    $at, -1
/* 0D9874 8031E874 14410004 */  bne   $v0, $at, .L_U_8031E888
/* 0D9878 8031E878 3C018000 */   lui   $at, 0x8000
/* 0D987C 8031E87C 15810002 */  bne   $t4, $at, .L_U_8031E888
/* 0D9880 8031E880 00000000 */   nop   
/* 0D9884 8031E884 0006000D */  break 6
.L_U_8031E888:
/* 0D9888 8031E888 AFA6003C */  sw    $a2, 0x3c($sp)
/* 0D988C 8031E88C 0C0C9768 */  jal   osAiGetLength
/* 0D9890 8031E890 00000000 */   nop   
/* 0D9894 8031E894 8FA6003C */  lw    $a2, 0x3c($sp)
/* 0D9898 8031E898 3C038022 */  lui   $v1, %hi(D_80226E4C)
/* 0D989C 8031E89C 3C078022 */  lui   $a3, %hi(D_80226D8C) # $a3, 0x8022
/* 0D98A0 8031E8A0 00066840 */  sll   $t5, $a2, 1
/* 0D98A4 8031E8A4 006D1821 */  addu  $v1, $v1, $t5
/* 0D98A8 8031E8A8 84636C4C */  lh    $v1, %lo(D_80226E4C)($v1)
/* 0D98AC 8031E8AC 24E76B8C */  addiu $a3, %lo(D_80226D8C) # addiu $a3, $a3, 0x6b8c
/* 0D98B0 8031E8B0 00025082 */  srl   $t2, $v0, 2
/* 0D98B4 8031E8B4 1060000A */  beqz  $v1, .L_U_8031E8E0
/* 0D98B8 8031E8B8 00067080 */   sll   $t6, $a2, 2
/* 0D98BC 8031E8BC 3C048022 */  lui   $a0, %hi(D_80226E40)
/* 0D98C0 8031E8C0 008E2021 */  addu  $a0, $a0, $t6
/* 0D98C4 8031E8C4 8C846C40 */  lw    $a0, %lo(D_80226E40)($a0)
/* 0D98C8 8031E8C8 00032880 */  sll   $a1, $v1, 2
/* 0D98CC 8031E8CC 0C0C976C */  jal   osAiSetNextBuffer
/* 0D98D0 8031E8D0 AFAA0044 */   sw    $t2, 0x44($sp)
/* 0D98D4 8031E8D4 3C078022 */  lui   $a3, %hi(D_80226D8C) # $a3, 0x8022
/* 0D98D8 8031E8D8 24E76B8C */  addiu $a3, %lo(D_80226D8C) # addiu $a3, $a3, 0x6b8c
/* 0D98DC 8031E8DC 8FAA0044 */  lw    $t2, 0x44($sp)
.L_U_8031E8E0:
/* 0D98E0 8031E8E0 3C0B8022 */  lui   $t3, %hi(D_80226D84) # $t3, 0x8022
/* 0D98E4 8031E8E4 256B6B84 */  addiu $t3, %lo(D_80226D84) # addiu $t3, $t3, 0x6b84
/* 0D98E8 8031E8E8 8D620000 */  lw    $v0, ($t3)
/* 0D98EC 8031E8EC AD600000 */  sw    $zero, ($t3)
/* 0D98F0 8031E8F0 3C028022 */  lui   $v0, %hi(D_80226D88) # $v0, 0x8022
/* 0D98F4 8031E8F4 8C426B88 */  lw    $v0, %lo(D_80226D88)($v0)
/* 0D98F8 8031E8F8 3C188022 */  lui   $t8, %hi(D_80226DA0) # $t8, 0x8022
/* 0D98FC 8031E8FC 3C098022 */  lui   $t1, %hi(D_80226D9C) # $t1, 0x8022
/* 0D9900 8031E900 00027880 */  sll   $t7, $v0, 2
/* 0D9904 8031E904 01E27821 */  addu  $t7, $t7, $v0
/* 0D9908 8031E908 000F7900 */  sll   $t7, $t7, 4
/* 0D990C 8031E90C 27186BA0 */  addiu $t8, %lo(D_80226DA0) # addiu $t8, $t8, 0x6ba0
/* 0D9910 8031E910 25296B9C */  addiu $t1, %lo(D_80226D9C) # addiu $t1, $t1, 0x6b9c
/* 0D9914 8031E914 01F8C821 */  addu  $t9, $t7, $t8
/* 0D9918 8031E918 3C0D8022 */  lui   $t5, %hi(D_80226D90)
/* 0D991C 8031E91C 00026080 */  sll   $t4, $v0, 2
/* 0D9920 8031E920 AD390000 */  sw    $t9, ($t1)
/* 0D9924 8031E924 01AC6821 */  addu  $t5, $t5, $t4
/* 0D9928 8031E928 8DAD6B90 */  lw    $t5, %lo(D_80226D90)($t5)
/* 0D992C 8031E92C 3C018022 */  lui   $at, %hi(D_80226D98) # $at, 0x8022
/* 0D9930 8031E930 3C048022 */  lui   $a0, %hi(D_80226D74) # $a0, 0x8022
/* 0D9934 8031E934 AC2D6B98 */  sw    $t5, %lo(D_80226D98)($at)
/* 0D9938 8031E938 8CE60000 */  lw    $a2, ($a3)
/* 0D993C 8031E93C 8C846B74 */  lw    $a0, %lo(D_80226D74)($a0)
/* 0D9940 8031E940 3C0F8022 */  lui   $t7, %hi(D_80226E40)
/* 0D9944 8031E944 00067080 */  sll   $t6, $a2, 2
/* 0D9948 8031E948 01EE7821 */  addu  $t7, $t7, $t6
/* 0D994C 8031E94C 8DEF6C40 */  lw    $t7, %lo(D_80226E40)($t7)
/* 0D9950 8031E950 008A6023 */  subu  $t4, $a0, $t2
/* 0D9954 8031E954 3C018036 */  lui   $at, %hi(D_80360120) # $at, 0x8036
/* 0D9958 8031E958 258D0040 */  addiu $t5, $t4, 0x40
/* 0D995C 8031E95C 3C198022 */  lui   $t9, %hi(D_80226E4C) # $t9, 0x8022
/* 0D9960 8031E960 27396C4C */  addiu $t9, %lo(D_80226E4C) # addiu $t9, $t9, 0x6c4c
/* 0D9964 8031E964 31AEFFF0 */  andi  $t6, $t5, 0xfff0
/* 0D9968 8031E968 0006C040 */  sll   $t8, $a2, 1
/* 0D996C 8031E96C AC2F1490 */  sw    $t7, %lo(D_80360120)($at)
/* 0D9970 8031E970 25CF0010 */  addiu $t7, $t6, 0x10
/* 0D9974 8031E974 03194021 */  addu  $t0, $t8, $t9
/* 0D9978 8031E978 A50F0000 */  sh    $t7, ($t0)
/* 0D997C 8031E97C 3C058022 */  lui   $a1, %hi(D_80226D78) # $a1, 0x8022
/* 0D9980 8031E980 8CA56B78 */  lw    $a1, %lo(D_80226D78)($a1)
/* 0D9984 8031E984 85030000 */  lh    $v1, ($t0)
/* 0D9988 8031E988 3C188033 */  lui   $t8, %hi(D_80331D44) # $t8, 0x8033
/* 0D998C 8031E98C 24820010 */  addiu $v0, $a0, 0x10
/* 0D9990 8031E990 0065082A */  slt   $at, $v1, $a1
/* 0D9994 8031E994 50200004 */  beql  $at, $zero, .L_U_8031E9A8
/* 0D9998 8031E998 0043082A */   slt   $at, $v0, $v1
/* 0D999C 8031E99C A5050000 */  sh    $a1, ($t0)
/* 0D99A0 8031E9A0 85030000 */  lh    $v1, ($t0)
/* 0D99A4 8031E9A4 0043082A */  slt   $at, $v0, $v1
.L_U_8031E9A8:
/* 0D99A8 8031E9A8 10200002 */  beqz  $at, .L_U_8031E9B4
/* 0D99AC 8031E9AC 00000000 */   nop   
/* 0D99B0 8031E9B0 A5020000 */  sh    $v0, ($t0)
.L_U_8031E9B4:
/* 0D99B4 8031E9B4 8F182E54 */  lw    $t8, %lo(D_80331D44)($t8)
/* 0D99B8 8031E9B8 13000006 */  beqz  $t8, .L_U_8031E9D4
/* 0D99BC 8031E9BC 00000000 */   nop   
/* 0D99C0 8031E9C0 0C0C7F6B */  jal   func_8031EEF8
/* 0D99C4 8031E9C4 AFA80028 */   sw    $t0, 0x28($sp)
/* 0D99C8 8031E9C8 3C018033 */  lui   $at, %hi(D_80331D44) # $at, 0x8033
/* 0D99CC 8031E9CC 8FA80028 */  lw    $t0, 0x28($sp)
/* 0D99D0 8031E9D0 AC202E54 */  sw    $zero, %lo(D_80331D44)($at)
.L_U_8031E9D4:
/* 0D99D4 8031E9D4 3C048022 */  lui   $a0, %hi(D_80226D98) # $a0, 0x8022
/* 0D99D8 8031E9D8 3C068036 */  lui   $a2, %hi(D_80360120) # $a2, 0x8036
/* 0D99DC 8031E9DC 8CC61490 */  lw    $a2, %lo(D_80360120)($a2)
/* 0D99E0 8031E9E0 8C846B98 */  lw    $a0, %lo(D_80226D98)($a0)
/* 0D99E4 8031E9E4 27A50040 */  addiu $a1, $sp, 0x40
/* 0D99E8 8031E9E8 0C0C5379 */  jal   func_80313CD4
/* 0D99EC 8031E9EC 85070000 */   lh    $a3, ($t0)
/* 0D99F0 8031E9F0 3C088022 */  lui   $t0, %hi(D_80226D80) # $t0, 0x8022
/* 0D99F4 8031E9F4 3C018022 */  lui   $at, %hi(D_80226D98) # $at, 0x8022
/* 0D99F8 8031E9F8 AC226B98 */  sw    $v0, %lo(D_80226D98)($at)
/* 0D99FC 8031E9FC 25086B80 */  addiu $t0, %lo(D_80226D80) # addiu $t0, $t0, 0x6b80
/* 0D9A00 8031EA00 3C048022 */  lui   $a0, %hi(D_80226EB8) # $a0, 0x8022
/* 0D9A04 8031EA04 8D190000 */  lw    $t9, ($t0)
/* 0D9A08 8031EA08 24846CB8 */  addiu $a0, %lo(D_80226EB8) # addiu $a0, $a0, 0x6cb8
/* 0D9A0C 8031EA0C 8C8C0000 */  lw    $t4, ($a0)
/* 0D9A10 8031EA10 8D0E0000 */  lw    $t6, ($t0)
/* 0D9A14 8031EA14 3C098022 */  lui   $t1, %hi(D_80226D9C) # $t1, 0x8022
/* 0D9A18 8031EA18 032C6821 */  addu  $t5, $t9, $t4
/* 0D9A1C 8031EA1C 01AE0019 */  multu $t5, $t6
/* 0D9A20 8031EA20 25296B9C */  addiu $t1, %lo(D_80226D9C) # addiu $t1, $t1, 0x6b9c
/* 0D9A24 8031EA24 8D380000 */  lw    $t8, ($t1)
/* 0D9A28 8031EA28 3C068022 */  lui   $a2, %hi(D_80226D88) # $a2, 0x8022
/* 0D9A2C 8031EA2C 3C058033 */  lui   $a1, %hi(rspF3DBootStart) # $a1, 0x8033
/* 0D9A30 8031EA30 3C0D8033 */  lui   $t5, %hi(rspF3DBootEnd) # $t5, 0x8033
/* 0D9A34 8031EA34 24A5B260 */  addiu $a1, %lo(rspF3DBootStart) # addiu $a1, $a1, -0x4da0
/* 0D9A38 8031EA38 3C078034 */  lui   $a3, %hi(rspAspMainDataStart) # $a3, 0x8034
/* 0D9A3C 8031EA3C 240C0002 */  li    $t4, 2
/* 0D9A40 8031EA40 25ADB330 */  addiu $t5, %lo(rspF3DBootEnd) # addiu $t5, $t5, -0x4cd0
/* 0D9A44 8031EA44 00007812 */  mflo  $t7
/* 0D9A48 8031EA48 AC8F0000 */  sw    $t7, ($a0)
/* 0D9A4C 8031EA4C 8CC66B88 */  lw    $a2, %lo(D_80226D88)($a2)
/* 0D9A50 8031EA50 AF000040 */  sw    $zero, 0x40($t8)
/* 0D9A54 8031EA54 8D390000 */  lw    $t9, ($t1)
/* 0D9A58 8031EA58 24E7A2C0 */  addiu $a3, %lo(rspAspMainDataStart) # addiu $a3, $a3, -0x5d40
/* 0D9A5C 8031EA5C 01A57023 */  subu  $t6, $t5, $a1
/* 0D9A60 8031EA60 AF200044 */  sw    $zero, 0x44($t9)
/* 0D9A64 8031EA64 8D230000 */  lw    $v1, ($t1)
/* 0D9A68 8031EA68 3C198034 */  lui   $t9, %hi(rspAspMainDataEnd) # $t9, 0x8034
/* 0D9A6C 8031EA6C 3C0F8033 */  lui   $t7, %hi(rspAspMainStart) # $t7, 0x8033
/* 0D9A70 8031EA70 2739A580 */  addiu $t9, %lo(rspAspMainDataEnd) # addiu $t9, $t9, -0x5a80
/* 0D9A74 8031EA74 AC6C0000 */  sw    $t4, ($v1)
/* 0D9A78 8031EA78 25EFC740 */  addiu $t7, %lo(rspAspMainStart) # addiu $t7, $t7, -0x38c0
/* 0D9A7C 8031EA7C 24180800 */  li    $t8, 2048
/* 0D9A80 8031EA80 03276023 */  subu  $t4, $t9, $a3
/* 0D9A84 8031EA84 000C68C3 */  sra   $t5, $t4, 3
/* 0D9A88 8031EA88 AC6E000C */  sw    $t6, 0xc($v1)
/* 0D9A8C 8031EA8C AC6F0010 */  sw    $t7, 0x10($v1)
/* 0D9A90 8031EA90 AC780014 */  sw    $t8, 0x14($v1)
/* 0D9A94 8031EA94 000D70C0 */  sll   $t6, $t5, 3
/* 0D9A98 8031EA98 3C188022 */  lui   $t8, %hi(D_80226D90)
/* 0D9A9C 8031EA9C 00067880 */  sll   $t7, $a2, 2
/* 0D9AA0 8031EAA0 030FC021 */  addu  $t8, $t8, $t7
/* 0D9AA4 8031EAA4 AC600004 */  sw    $zero, 4($v1)
/* 0D9AA8 8031EAA8 AC650008 */  sw    $a1, 8($v1)
/* 0D9AAC 8031EAAC AC670018 */  sw    $a3, 0x18($v1)
/* 0D9AB0 8031EAB0 AC6E001C */  sw    $t6, 0x1c($v1)
/* 0D9AB4 8031EAB4 AC600020 */  sw    $zero, 0x20($v1)
/* 0D9AB8 8031EAB8 AC600024 */  sw    $zero, 0x24($v1)
/* 0D9ABC 8031EABC AC600028 */  sw    $zero, 0x28($v1)
/* 0D9AC0 8031EAC0 AC60002C */  sw    $zero, 0x2c($v1)
/* 0D9AC4 8031EAC4 8F186B90 */  lw    $t8, %lo(D_80226D90)($t8)
/* 0D9AC8 8031EAC8 AC780030 */  sw    $t8, 0x30($v1)
/* 0D9ACC 8031EACC 8FB90040 */  lw    $t9, 0x40($sp)
/* 0D9AD0 8031EAD0 AC600038 */  sw    $zero, 0x38($v1)
/* 0D9AD4 8031EAD4 AC60003C */  sw    $zero, 0x3c($v1)
/* 0D9AD8 8031EAD8 001960C0 */  sll   $t4, $t9, 3
/* 0D9ADC 8031EADC 0C0C607B */  jal   func_8031715C
/* 0D9AE0 8031EAE0 AC6C0034 */   sw    $t4, 0x34($v1)
/* 0D9AE4 8031EAE4 3C098022 */  lui   $t1, %hi(D_80226D9C) # $t1, 0x8022
/* 0D9AE8 8031EAE8 25296B9C */  addiu $t1, %lo(D_80226D9C) # addiu $t1, $t1, 0x6b9c
/* 0D9AEC 8031EAEC 8D220000 */  lw    $v0, ($t1)
.L_U_8031EAF0:
/* 0D9AF0 8031EAF0 8FBF0014 */  lw    $ra, 0x14($sp)
/* 0D9AF4 8031EAF4 27BD0048 */  addiu $sp, $sp, 0x48
/* 0D9AF8 8031EAF8 03E00008 */  jr    $ra
/* 0D9AFC 8031EAFC 00000000 */   nop   
