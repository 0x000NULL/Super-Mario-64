glabel func_8031D924
/* 0D8924 8031D924 3C048022 */  lui   $a0, %hi(D_80226D80) # $a0, 0x8022
/* 0D8928 8031D928 24846D80 */  addiu $a0, %lo(D_80226D80) # addiu $a0, $a0, 0x6d80
/* 0D892C 8031D92C 8C820000 */  lw    $v0, ($a0)
/* 0D8930 8031D930 27BDFFB8 */  addiu $sp, $sp, -0x48
/* 0D8934 8031D934 3C0F8033 */  lui   $t7, %hi(D_80333EF4) # $t7, 0x8033
/* 0D8938 8031D938 244E0001 */  addiu $t6, $v0, 1
/* 0D893C 8031D93C AFBF0014 */  sw    $ra, 0x14($sp)
/* 0D8940 8031D940 AC8E0000 */  sw    $t6, ($a0)
/* 0D8944 8031D944 25EF3EF4 */  addiu $t7, %lo(D_80333EF4) # addiu $t7, $t7, 0x3ef4
/* 0D8948 8031D948 8DF80000 */  lw    $t8, ($t7)
/* 0D894C 8031D94C 3C017655 */  lui   $at, (0x76557364 >> 16) # lui $at, 0x7655
/* 0D8950 8031D950 34217364 */  ori   $at, (0x76557364 & 0xFFFF) # ori $at, $at, 0x7364
/* 0D8954 8031D954 13010003 */  beq   $t8, $at, .L8031D964
/* 0D8958 8031D958 24020003 */   li    $v0, 3
/* 0D895C 8031D95C 100000C2 */  b     .L8031DC68
/* 0D8960 8031D960 00001025 */   move  $v0, $zero
.L8031D964:
/* 0D8964 8031D964 3C078022 */  lui   $a3, %hi(D_80226D8C) # $a3, 0x8022
/* 0D8968 8031D968 24E76D8C */  addiu $a3, %lo(D_80226D8C) # addiu $a3, $a3, 0x6d8c
/* 0D896C 8031D96C 8CED0000 */  lw    $t5, ($a3)
/* 0D8970 8031D970 3C038022 */  lui   $v1, %hi(D_80226D88) # $v1, 0x8022
/* 0D8974 8031D974 24636D88 */  addiu $v1, %lo(D_80226D88) # addiu $v1, $v1, 0x6d88
/* 0D8978 8031D978 25AE0001 */  addiu $t6, $t5, 1
/* 0D897C 8031D97C 01C2001A */  div   $zero, $t6, $v0
/* 0D8980 8031D980 8C790000 */  lw    $t9, ($v1)
/* 0D8984 8031D984 0000C010 */  mfhi  $t8
/* 0D8988 8031D988 01C07825 */  move  $t7, $t6
/* 0D898C 8031D98C 3B2C0001 */  xori  $t4, $t9, 1
/* 0D8990 8031D990 AC6C0000 */  sw    $t4, ($v1)
/* 0D8994 8031D994 270C0001 */  addiu $t4, $t8, 1
/* 0D8998 8031D998 0182001A */  div   $zero, $t4, $v0
/* 0D899C 8031D99C ACEE0000 */  sw    $t6, ($a3)
/* 0D89A0 8031D9A0 14400002 */  bnez  $v0, .L8031D9AC
/* 0D89A4 8031D9A4 00000000 */   nop   
/* 0D89A8 8031D9A8 0007000D */  break 7
.L8031D9AC:
/* 0D89AC 8031D9AC 2401FFFF */  li    $at, -1
/* 0D89B0 8031D9B0 14410004 */  bne   $v0, $at, .L8031D9C4
/* 0D89B4 8031D9B4 3C018000 */   lui   $at, 0x8000
/* 0D89B8 8031D9B8 15E10002 */  bne   $t7, $at, .L8031D9C4
/* 0D89BC 8031D9BC 00000000 */   nop   
/* 0D89C0 8031D9C0 0006000D */  break 6
.L8031D9C4:
/* 0D89C4 8031D9C4 00003010 */  mfhi  $a2
/* 0D89C8 8031D9C8 ACF80000 */  sw    $t8, ($a3)
/* 0D89CC 8031D9CC 0300C825 */  move  $t9, $t8
/* 0D89D0 8031D9D0 14400002 */  bnez  $v0, .L8031D9DC
/* 0D89D4 8031D9D4 00000000 */   nop   
/* 0D89D8 8031D9D8 0007000D */  break 7
.L8031D9DC:
/* 0D89DC 8031D9DC 2401FFFF */  li    $at, -1
/* 0D89E0 8031D9E0 14410004 */  bne   $v0, $at, .L8031D9F4
/* 0D89E4 8031D9E4 3C018000 */   lui   $at, 0x8000
/* 0D89E8 8031D9E8 15810002 */  bne   $t4, $at, .L8031D9F4
/* 0D89EC 8031D9EC 00000000 */   nop   
/* 0D89F0 8031D9F0 0006000D */  break 6
.L8031D9F4:
/* 0D89F4 8031D9F4 AFA6003C */  sw    $a2, 0x3c($sp)
/* 0D89F8 8031D9F8 0C0C939C */  jal   osAiGetLength
/* 0D89FC 8031D9FC 00000000 */   nop   
/* 0D8A00 8031DA00 8FA6003C */  lw    $a2, 0x3c($sp)
/* 0D8A04 8031DA04 3C038022 */  lui   $v1, %hi(D_80226E4C)
/* 0D8A08 8031DA08 3C078022 */  lui   $a3, %hi(D_80226D8C) # $a3, 0x8022
/* 0D8A0C 8031DA0C 00066840 */  sll   $t5, $a2, 1
/* 0D8A10 8031DA10 006D1821 */  addu  $v1, $v1, $t5
/* 0D8A14 8031DA14 84636E4C */  lh    $v1, %lo(D_80226E4C)($v1)
/* 0D8A18 8031DA18 24E76D8C */  addiu $a3, %lo(D_80226D8C) # addiu $a3, $a3, 0x6d8c
/* 0D8A1C 8031DA1C 00025082 */  srl   $t2, $v0, 2
/* 0D8A20 8031DA20 1060000A */  beqz  $v1, .L8031DA4C
/* 0D8A24 8031DA24 00067080 */   sll   $t6, $a2, 2
/* 0D8A28 8031DA28 3C048022 */  lui   $a0, %hi(D_80226E40)
/* 0D8A2C 8031DA2C 008E2021 */  addu  $a0, $a0, $t6
/* 0D8A30 8031DA30 8C846E40 */  lw    $a0, %lo(D_80226E40)($a0)
/* 0D8A34 8031DA34 00032880 */  sll   $a1, $v1, 2
/* 0D8A38 8031DA38 0C0C93A0 */  jal   osAiSetNextBuffer
/* 0D8A3C 8031DA3C AFAA0044 */   sw    $t2, 0x44($sp)
/* 0D8A40 8031DA40 3C078022 */  lui   $a3, %hi(D_80226D8C) # $a3, 0x8022
/* 0D8A44 8031DA44 24E76D8C */  addiu $a3, %lo(D_80226D8C) # addiu $a3, $a3, 0x6d8c
/* 0D8A48 8031DA48 8FAA0044 */  lw    $t2, 0x44($sp)
.L8031DA4C:
/* 0D8A4C 8031DA4C 3C0B8022 */  lui   $t3, %hi(D_80226D84) # $t3, 0x8022
/* 0D8A50 8031DA50 256B6D84 */  addiu $t3, %lo(D_80226D84) # addiu $t3, $t3, 0x6d84
/* 0D8A54 8031DA54 8D620000 */  lw    $v0, ($t3)
/* 0D8A58 8031DA58 AD600000 */  sw    $zero, ($t3)
/* 0D8A5C 8031DA5C 3C028022 */  lui   $v0, %hi(D_80226D88) # $v0, 0x8022
/* 0D8A60 8031DA60 8C426D88 */  lw    $v0, %lo(D_80226D88)($v0)
/* 0D8A64 8031DA64 3C188022 */  lui   $t8, %hi(D_80226DA0) # $t8, 0x8022
/* 0D8A68 8031DA68 3C098022 */  lui   $t1, %hi(D_80226D9C) # $t1, 0x8022
/* 0D8A6C 8031DA6C 00027880 */  sll   $t7, $v0, 2
/* 0D8A70 8031DA70 01E27821 */  addu  $t7, $t7, $v0
/* 0D8A74 8031DA74 000F7900 */  sll   $t7, $t7, 4
/* 0D8A78 8031DA78 27186DA0 */  addiu $t8, %lo(D_80226DA0) # addiu $t8, $t8, 0x6da0
/* 0D8A7C 8031DA7C 25296D9C */  addiu $t1, %lo(D_80226D9C) # addiu $t1, $t1, 0x6d9c
/* 0D8A80 8031DA80 01F8C821 */  addu  $t9, $t7, $t8
/* 0D8A84 8031DA84 3C0D8022 */  lui   $t5, %hi(D_80226D90)
/* 0D8A88 8031DA88 00026080 */  sll   $t4, $v0, 2
/* 0D8A8C 8031DA8C AD390000 */  sw    $t9, ($t1)
/* 0D8A90 8031DA90 01AC6821 */  addu  $t5, $t5, $t4
/* 0D8A94 8031DA94 8DAD6D90 */  lw    $t5, %lo(D_80226D90)($t5)
/* 0D8A98 8031DA98 3C018022 */  lui   $at, %hi(D_80226D98) # $at, 0x8022
/* 0D8A9C 8031DA9C 3C048022 */  lui   $a0, %hi(D_80226D74) # $a0, 0x8022
/* 0D8AA0 8031DAA0 AC2D6D98 */  sw    $t5, %lo(D_80226D98)($at)
/* 0D8AA4 8031DAA4 8CE60000 */  lw    $a2, ($a3)
/* 0D8AA8 8031DAA8 8C846D74 */  lw    $a0, %lo(D_80226D74)($a0)
/* 0D8AAC 8031DAAC 3C0F8022 */  lui   $t7, %hi(D_80226E40)
/* 0D8AB0 8031DAB0 00067080 */  sll   $t6, $a2, 2
/* 0D8AB4 8031DAB4 01EE7821 */  addu  $t7, $t7, $t6
/* 0D8AB8 8031DAB8 8DEF6E40 */  lw    $t7, %lo(D_80226E40)($t7)
/* 0D8ABC 8031DABC 008A6023 */  subu  $t4, $a0, $t2
/* 0D8AC0 8031DAC0 3C018036 */  lui   $at, %hi(D_80360120) # $at, 0x8036
/* 0D8AC4 8031DAC4 258D0040 */  addiu $t5, $t4, 0x40
/* 0D8AC8 8031DAC8 3C198022 */  lui   $t9, %hi(D_80226E4C) # $t9, 0x8022
/* 0D8ACC 8031DACC 27396E4C */  addiu $t9, %lo(D_80226E4C) # addiu $t9, $t9, 0x6e4c
/* 0D8AD0 8031DAD0 31AEFFF0 */  andi  $t6, $t5, 0xfff0
/* 0D8AD4 8031DAD4 0006C040 */  sll   $t8, $a2, 1
/* 0D8AD8 8031DAD8 AC2F0120 */  sw    $t7, %lo(D_80360120)($at)
/* 0D8ADC 8031DADC 25CF0010 */  addiu $t7, $t6, 0x10
/* 0D8AE0 8031DAE0 03194021 */  addu  $t0, $t8, $t9
/* 0D8AE4 8031DAE4 A50F0000 */  sh    $t7, ($t0)
/* 0D8AE8 8031DAE8 3C058022 */  lui   $a1, %hi(D_80226D78) # $a1, 0x8022
/* 0D8AEC 8031DAEC 8CA56D78 */  lw    $a1, %lo(D_80226D78)($a1)
/* 0D8AF0 8031DAF0 85030000 */  lh    $v1, ($t0)
/* 0D8AF4 8031DAF4 3C188033 */  lui   $t8, %hi(D_80331D44) # $t8, 0x8033
/* 0D8AF8 8031DAF8 24820010 */  addiu $v0, $a0, 0x10
/* 0D8AFC 8031DAFC 0065082A */  slt   $at, $v1, $a1
/* 0D8B00 8031DB00 50200004 */  beql  $at, $zero, .L8031DB14
/* 0D8B04 8031DB04 0043082A */   slt   $at, $v0, $v1
/* 0D8B08 8031DB08 A5050000 */  sh    $a1, ($t0)
/* 0D8B0C 8031DB0C 85030000 */  lh    $v1, ($t0)
/* 0D8B10 8031DB10 0043082A */  slt   $at, $v0, $v1
.L8031DB14:
/* 0D8B14 8031DB14 10200002 */  beqz  $at, .L8031DB20
/* 0D8B18 8031DB18 00000000 */   nop   
/* 0D8B1C 8031DB1C A5020000 */  sh    $v0, ($t0)
.L8031DB20:
/* 0D8B20 8031DB20 8F181D44 */  lw    $t8, %lo(D_80331D44)($t8)
/* 0D8B24 8031DB24 13000006 */  beqz  $t8, .L8031DB40
/* 0D8B28 8031DB28 00000000 */   nop   
/* 0D8B2C 8031DB2C 0C0C7BBE */  jal   func_8031EEF8
/* 0D8B30 8031DB30 AFA80028 */   sw    $t0, 0x28($sp)
/* 0D8B34 8031DB34 3C018033 */  lui   $at, %hi(D_80331D44) # $at, 0x8033
/* 0D8B38 8031DB38 8FA80028 */  lw    $t0, 0x28($sp)
/* 0D8B3C 8031DB3C AC201D44 */  sw    $zero, %lo(D_80331D44)($at)
.L8031DB40:
/* 0D8B40 8031DB40 3C048022 */  lui   $a0, %hi(D_80226D98) # $a0, 0x8022
/* 0D8B44 8031DB44 3C068036 */  lui   $a2, %hi(D_80360120) # $a2, 0x8036
/* 0D8B48 8031DB48 8CC60120 */  lw    $a2, %lo(D_80360120)($a2)
/* 0D8B4C 8031DB4C 8C846D98 */  lw    $a0, %lo(D_80226D98)($a0)
/* 0D8B50 8031DB50 27A50040 */  addiu $a1, $sp, 0x40
/* 0D8B54 8031DB54 0C0C4F35 */  jal   func_80313CD4
/* 0D8B58 8031DB58 85070000 */   lh    $a3, ($t0)
/* 0D8B5C 8031DB5C 3C088022 */  lui   $t0, %hi(D_80226D80) # $t0, 0x8022
/* 0D8B60 8031DB60 3C018022 */  lui   $at, %hi(D_80226D98) # $at, 0x8022
/* 0D8B64 8031DB64 AC226D98 */  sw    $v0, %lo(D_80226D98)($at)
/* 0D8B68 8031DB68 25086D80 */  addiu $t0, %lo(D_80226D80) # addiu $t0, $t0, 0x6d80
/* 0D8B6C 8031DB6C 3C048022 */  lui   $a0, %hi(D_80226EB8) # $a0, 0x8022
/* 0D8B70 8031DB70 8D190000 */  lw    $t9, ($t0)
/* 0D8B74 8031DB74 24846EB8 */  addiu $a0, %lo(D_80226EB8) # addiu $a0, $a0, 0x6eb8
/* 0D8B78 8031DB78 8C8C0000 */  lw    $t4, ($a0)
/* 0D8B7C 8031DB7C 8D0E0000 */  lw    $t6, ($t0)
/* 0D8B80 8031DB80 3C098022 */  lui   $t1, %hi(D_80226D9C) # $t1, 0x8022
/* 0D8B84 8031DB84 032C6821 */  addu  $t5, $t9, $t4
/* 0D8B88 8031DB88 01AE0019 */  multu $t5, $t6
/* 0D8B8C 8031DB8C 25296D9C */  addiu $t1, %lo(D_80226D9C) # addiu $t1, $t1, 0x6d9c
/* 0D8B90 8031DB90 8D380000 */  lw    $t8, ($t1)
/* 0D8B94 8031DB94 3C068022 */  lui   $a2, %hi(D_80226D88) # $a2, 0x8022
/* 0D8B98 8031DB98 3C058033 */  lui   $a1, %hi(rspF3DBootStart) # $a1, 0x8033
/* 0D8B9C 8031DB9C 3C0D8033 */  lui   $t5, %hi(rspF3DBootEnd) # $t5, 0x8033
/* 0D8BA0 8031DBA0 24A5A320 */  addiu $a1, %lo(rspF3DBootStart) # addiu $a1, $a1, -0x5ce0
/* 0D8BA4 8031DBA4 3C078034 */  lui   $a3, %hi(rspAspMainDataStart) # $a3, 0x8034
/* 0D8BA8 8031DBA8 240C0002 */  li    $t4, 2
/* 0D8BAC 8031DBAC 25ADA3F0 */  addiu $t5, %lo(rspF3DBootEnd) # addiu $t5, $t5, -0x5c10
/* 0D8BB0 8031DBB0 00007812 */  mflo  $t7
/* 0D8BB4 8031DBB4 AC8F0000 */  sw    $t7, ($a0)
/* 0D8BB8 8031DBB8 8CC66D88 */  lw    $a2, %lo(D_80226D88)($a2)
/* 0D8BBC 8031DBBC AF000040 */  sw    $zero, 0x40($t8)
/* 0D8BC0 8031DBC0 8D390000 */  lw    $t9, ($t1)
/* 0D8BC4 8031DBC4 24E78F50 */  addiu $a3, %lo(rspAspMainDataStart) # addiu $a3, $a3, -0x70b0
/* 0D8BC8 8031DBC8 01A57023 */  subu  $t6, $t5, $a1
/* 0D8BCC 8031DBCC AF200044 */  sw    $zero, 0x44($t9)
/* 0D8BD0 8031DBD0 8D230000 */  lw    $v1, ($t1)
/* 0D8BD4 8031DBD4 3C198034 */  lui   $t9, %hi(rspAspMainDataEnd) # $t9, 0x8034
/* 0D8BD8 8031DBD8 3C0F8033 */  lui   $t7, %hi(rspAspMainStart) # $t7, 0x8033
/* 0D8BDC 8031DBDC 27399210 */  addiu $t9, %lo(rspAspMainDataEnd) # addiu $t9, $t9, -0x6df0
/* 0D8BE0 8031DBE0 AC6C0000 */  sw    $t4, ($v1)
/* 0D8BE4 8031DBE4 25EFB800 */  addiu $t7, %lo(rspAspMainStart) # addiu $t7, $t7, -0x4800
/* 0D8BE8 8031DBE8 24180800 */  li    $t8, 2048
/* 0D8BEC 8031DBEC 03276023 */  subu  $t4, $t9, $a3
/* 0D8BF0 8031DBF0 000C68C3 */  sra   $t5, $t4, 3
/* 0D8BF4 8031DBF4 AC6E000C */  sw    $t6, 0xc($v1)
/* 0D8BF8 8031DBF8 AC6F0010 */  sw    $t7, 0x10($v1)
/* 0D8BFC 8031DBFC AC780014 */  sw    $t8, 0x14($v1)
/* 0D8C00 8031DC00 000D70C0 */  sll   $t6, $t5, 3
/* 0D8C04 8031DC04 3C188022 */  lui   $t8, %hi(D_80226D90)
/* 0D8C08 8031DC08 00067880 */  sll   $t7, $a2, 2
/* 0D8C0C 8031DC0C 030FC021 */  addu  $t8, $t8, $t7
/* 0D8C10 8031DC10 AC600004 */  sw    $zero, 4($v1)
/* 0D8C14 8031DC14 AC650008 */  sw    $a1, 8($v1)
/* 0D8C18 8031DC18 AC670018 */  sw    $a3, 0x18($v1)
/* 0D8C1C 8031DC1C AC6E001C */  sw    $t6, 0x1c($v1)
/* 0D8C20 8031DC20 AC600020 */  sw    $zero, 0x20($v1)
/* 0D8C24 8031DC24 AC600024 */  sw    $zero, 0x24($v1)
/* 0D8C28 8031DC28 AC600028 */  sw    $zero, 0x28($v1)
/* 0D8C2C 8031DC2C AC60002C */  sw    $zero, 0x2c($v1)
/* 0D8C30 8031DC30 8F186D90 */  lw    $t8, %lo(D_80226D90)($t8)
/* 0D8C34 8031DC34 3C0D8020 */  lui   $t5, %hi(gAudioSPTaskYieldBuffer) # $t5, 0x8020
/* 0D8C38 8031DC38 25ADF200 */  addiu $t5, %lo(gAudioSPTaskYieldBuffer) # addiu $t5, $t5, -0xe00
/* 0D8C3C 8031DC3C AC780030 */  sw    $t8, 0x30($v1)
/* 0D8C40 8031DC40 8FB90040 */  lw    $t9, 0x40($sp)
/* 0D8C44 8031DC44 240E0400 */  li    $t6, 1024
/* 0D8C48 8031DC48 AC6D0038 */  sw    $t5, 0x38($v1)
/* 0D8C4C 8031DC4C 001960C0 */  sll   $t4, $t9, 3
/* 0D8C50 8031DC50 AC6C0034 */  sw    $t4, 0x34($v1)
/* 0D8C54 8031DC54 0C0C5C57 */  jal   func_8031715C
/* 0D8C58 8031DC58 AC6E003C */   sw    $t6, 0x3c($v1)
/* 0D8C5C 8031DC5C 3C098022 */  lui   $t1, %hi(D_80226D9C) # $t1, 0x8022
/* 0D8C60 8031DC60 25296D9C */  addiu $t1, %lo(D_80226D9C) # addiu $t1, $t1, 0x6d9c
/* 0D8C64 8031DC64 8D220000 */  lw    $v0, ($t1)
.L8031DC68:
/* 0D8C68 8031DC68 8FBF0014 */  lw    $ra, 0x14($sp)
/* 0D8C6C 8031DC6C 27BD0048 */  addiu $sp, $sp, 0x48
/* 0D8C70 8031DC70 03E00008 */  jr    $ra
/* 0D8C74 8031DC74 00000000 */   nop   
