# Collision Macros

# Collision Data Routine Initiate
.macro colInit
	.hword 0x0040
.endm

# Collision Verteces Read Initiate
.macro colVertexInit vtxNum
	.hword \vtxNum
.endm

# Collision Vertex
.macro colVertex x, y, z
	.hword \x, \y, \z
.endm

# Collision Surface Types (shamelessly copied from Hack64 wiki)
.set SURF_ENV_DEFAULT,  0x00  # Environment default
.set SURF_BURNING,      0x01  # Burn / Frostbite
.set SURF_04,           0x04  # Unused
.set SURF_CEILING,      0x05  # Ceiling that Mario can climb on
.set SURF_SLOW,         0x09  # Slow down Mario
.set SURF_DEATH_FLOOR,  0x0A  # Death floor
.set SURF_CLOSE_CAM,    0x0B  # Close camera
.set SURF_WATER,        0x0D  # Water
.set SURF_WATER_FLOW,   0x0E  # Water (flowing)
.set SURF_INTANGIBLE,   0x12  # Intangible (Seperates BBH mansion from merry-go-round, for room usage)
.set SURF_SLIPPERY,     0x13  # Slippery
.set SURF_SLIGHT_SLIP,  0x14  # Slippery (slightly)
.set SURF_NO_SLIP,      0x15  # Anti-slippery
.set SURF_UNK_16,       0x16  # Unknown, used in Tall Tall Mountain
.set SURF_VARIED_NOISE, 0x1A  # Varied noise depending on terrain (mostly unused)
.set SURF_INST_WARP_1B, 0x1B  # Instant warp to another area
.set SURF_INST_WARP_1C, 0x1C  # Instant warp to another area
.set SURF_INST_WARP_1D, 0x1D  # Instant warp to another area
.set SURF_INST_WARP_1E, 0x1E  # Instant warp to another area
.set SURF_SAND_21,      0x21  # Sand (depth of 10 units)
.set SURF_QUICKSAND_22, 0x22  # Quicksand (lethal, slow, depth of 160 units)
.set SURF_QUICKSAND_23, 0x23  # Quicksand (lethal, instant)
.set SURF_QUICKSAND_24, 0x24  # Moving quicksand (flowing, depth of 160 units)
.set SURF_QUICKSAND_25, 0x25  # Moving quicksand (flowing, depth of 25 units)
.set SURF_QUICKSAND_26, 0x26  # Moving quicksand (60 units)
.set SURF_QUICKSAND_27, 0x27  # Moving quicksand (flowing, depth of 60 units)
.set SURF_WALL_ETC,     0x28  # Wall / Fence / Cannon
.set SURF_NOISE_DEF,    0x29  # Default floor with noise
.set SURF_NOISE_SLIP,   0x2A  # Slippery floor with noise
.set SURF_WIND,         0x2C  # Wind
.set SURF_QUICKSAND_2D, 0x2D  # Quicksand (lethal, flowing)
.set SURF_SLIP_ICE,     0x2E  # Slippery Ice (CCM slide)
.set SURF_LOOK_WARP,    0x2F  # Look up and warp (Wing cap entrance)
.set SURF_HARD_FLOOR,   0x30  # Hard floor (Always has fall damage)
.set SURF_WARP,         0x32  # Surface warp
.set SURF_TIMER_ON,     0x33  # Timer start (Peach's secret slide)
.set SURF_TIMER_OFF,    0x34  # Timer stop (Peach's secret slide)
.set SURF_HARD_SLIP,    0x35  # Hard and slippery (Always has fall damage)
.set SURF_HARD,         0x36  # Hard (Slide for CCM, Always has fall damage)
.set SURF_NO_SLIP_ICE,  0x37  # Non-slippery areas in ice levels
.set SURF_DEATH_WIND,   0x38  # Death at bottom with wind
.set SURF_WIDE_CAM,     0x65  # Wide camera (BOB)
.set SURF_WALLS_66,     0x66  # Walls in THI area 3
.set SURF_UNK_68,       0x68  # Unknown, used in Tiny Huge Island
.set SURF_UNK_69,       0x69  # Unknown, used in Tiny Huge Island
.set SURF_PYR_TOP_BOOM, 0x6E  # Step on 4 to make Pyramid top explode
.set SURF_CAM_6F,       0x6F  # Camera-related (Bowser 1)
.set SURF_CAM_70,       0x70  # Camera-related (BOB)
.set SURF_NO_ACCESS,    0x72  # Inaccessible Area, only used to restrict camera movement
.set SURF_NOISE_SLD_73, 0x73  # Slide with noise (unused)
.set SURF_NOISE_SLD_74, 0x74  # Slide with noise (unused)
.set SURF_CAM_75,       0x75  # Camera-related (CCM)
.set SURF_FLAG_SURF_76, 0x76  # Surface with flags
.set SURF_FLAG_SURF_77, 0x77  # Surface with flags (unused)
.set SURF_UNK_NOISE,    0x78  # Possibly for camera behavior, has noise
.set SURF_SLIPPERY_79,  0x79  # Slippery (for camera behavior)
.set SURF_ACTIVATE,     0x7A  # Activate switches or Dorrie
.set SURF_VAN_CAP_WALL, 0x7B  # Vanish cap walls
.set SURF_PAINTING_A6,  0xA6  # Painting wobble (BoB 1)
.set SURF_PAINTING_A7,  0xA7  # Painting wobble (BoB 2)
.set SURF_PAINTING_A8,  0xA8  # Painting wobble (BoB 3)
.set SURF_PAINTING_A9,  0xA9  # Painting wobble (CCM 1)	
.set SURF_PAINTING_AA,  0xAA  # Painting wobble (CCM 2)
.set SURF_PAINTING_AB,  0xAB  # Painting wobble (CCM 3)
.set SURF_PAINTING_AC,  0xAC  # Painting wobble (WF 1)
.set SURF_PAINTING_AD,  0xAD  # Painting wobble (WF 2)
.set SURF_PAINTING_AE,  0xAE  # Painting wobble (WF 3)
.set SURF_PAINTING_AF,  0xAF  # Painting wobble (JRB 1)
.set SURF_PAINTING_B0,  0xB0  # Painting wobble (JRB 2)
.set SURF_PAINTING_B1,  0xB1  # Painting wobble (JRB 3)
.set SURF_PAINTING_B2,  0xB2  # Painting wobble (LLL 1)
.set SURF_PAINTING_B3,  0xB3  # Painting wobble (LLL 2)
.set SURF_PAINTING_B4,  0xB4  # Painting wobble (LLL 3)
.set SURF_PAINTING_B5,  0xB5  # Painting wobble (SSL 1)
.set SURF_PAINTING_B6,  0xB6  # Painting wobble (SSL 2)
.set SURF_PAINTING_B7,  0xB7  # Painting wobble (SSL 3)
.set SURF_PAINTING_B8,  0xB8  # Painting wobble (?)
.set SURF_PAINTING_B9,  0xB9  # Painting wobble (?)
.set SURF_PAINTING_BA,  0xBA  # Painting wobble (?)
.set SURF_PAINTING_BB,  0xBB  # Painting wobble (BFS?)
.set SURF_PAINTING_BC,  0xBC  # Painting wobble (BFS?)
.set SURF_PAINTING_BD,  0xBD  # Painting wobble (BFS?)
.set SURF_PAINTING_BE,  0xBE  # Painting wobble (WDW 1)
.set SURF_PAINTING_BF,  0xBF  # Painting wobble (WDW 2)
.set SURF_PAINTING_C0,  0xC0  # Painting wobble (WDW 3)
.set SURF_PAINTING_C1,  0xC1  # Painting wobble (THI t 1)
.set SURF_PAINTING_C2,  0xC2  # Painting wobble (THI t 2)
.set SURF_PAINTING_C3,  0xC3  # Painting wobble (THI t 3)
.set SURF_PAINTING_C4,  0xC4  # Painting wobble (TTM 1)
.set SURF_PAINTING_C5,  0xC5  # Painting wobble (TTM 2)
.set SURF_PAINTING_C6,  0xC6  # Painting wobble (TTM 3)
.set SURF_PAINTING_C7,  0xC7  # Painting wobble (?)
.set SURF_PAINTING_C8,  0xC8  # Painting wobble (?)
.set SURF_PAINTING_C9,  0xC9  # Painting wobble (?)
.set SURF_PAINTING_CA,  0xCA  # Painting wobble (SML 1, unused)
.set SURF_PAINTING_CB,  0xCB  # Painting wobble (SML 2, unused)
.set SURF_PAINTING_CC,  0xCC  # Painting wobble (SML 3, unused)
.set SURF_PAINTING_CD,  0xCD  # Painting wobble (THI h 1)
.set SURF_PAINTING_CE,  0xCE  # Painting wobble (THI h 2)
.set SURF_PAINTING_CF,  0xCF  # Painting wobble (THI h 3)
.set SURF_PAINTING_D0,  0xD0  # Painting wobble (Metal cap?)
.set SURF_PAINTING_D1,  0xD1  # Painting wobble (Metal cap?)
.set SURF_PAINTING_D2,  0xD2  # Painting wobble (Metal cap?)
.set SURF_H_LVL_EN_D3,  0xD3  # Horizontal level entrance
.set SURF_H_LVL_EN_D4,  0xD4  # Horizontal level entrance
.set SURF_H_LVL_EN_D5,  0xD5  # Horizontal level entrance
.set SURF_H_LVL_EN_D6,  0xD6  # Horizontal level entrance
.set SURF_H_LVL_EN_D7,  0xD7  # Horizontal level entrance
.set SURF_H_LVL_EN_D8,  0xD8  # Horizontal level entrance
.set SURF_H_LVL_EN_D9,  0xD9  # Horizontal level entrance
.set SURF_H_LVL_EN_DA,  0xDA  # Horizontal level entrance
.set SURF_H_LVL_EN_DB,  0xDB  # Horizontal level entrance
.set SURF_H_LVL_EN_DC,  0xDC  # Horizontal level entrance
.set SURF_H_LVL_EN_DD,  0xDD  # Horizontal level entrance
.set SURF_H_LVL_EN_DE,  0xDE  # Horizontal level entrance
.set SURF_H_LVL_EN_DF,  0xDF  # Horizontal level entrance
.set SURF_H_LVL_EN_E0,  0xE0  # Horizontal level entrance
.set SURF_H_LVL_EN_E1,  0xE1  # Horizontal level entrance
.set SURF_H_LVL_EN_E2,  0xE2  # Horizontal level entrance
.set SURF_H_LVL_EN_E3,  0xE3  # Horizontal level entrance
.set SURF_H_LVL_EN_E4,  0xE4  # Horizontal level entrance
.set SURF_H_LVL_EN_E8,  0xE8  # Horizontal level entrance
.set SURF_H_LVL_EN_E9,  0xE9  # Horizontal level entrance
.set SURF_H_LVL_EN_EA,  0xEA  # Horizontal level entrance
.set SURF_H_LVL_EN_EB,  0xEB  # Horizontal level entrance
.set SURF_H_LVL_EN_EC,  0xEC  # Horizontal level entrance
.set SURF_H_LVL_EN_ED,  0xED  # Horizontal level entrance
.set SURF_H_LVL_EN_EE,  0xEE  # Horizontal level entrance
.set SURF_H_LVL_EN_EF,  0xEF  # Horizontal level entrance
.set SURF_H_LVL_EN_F0,  0xF0  # Horizontal level entrance
.set SURF_H_LVL_EN_F1,  0xF1  # Horizontal level entrance
.set SURF_H_LVL_EN_F2,  0xF2  # Horizontal level entrance
.set SURF_H_LVL_EN_F3,  0xF3  # Horizontal level entrance
.set SURF_H_LVL_EN_F4,  0xF4  # Horizontal level entrance
.set SURF_H_LVL_EN_F5,  0xF5  # Horizontal level entrance
.set SURF_H_LVL_EN_F6,  0xF6  # Horizontal level entrance
.set SURF_H_LVL_EN_F7,  0xF7  # Horizontal level entrance
.set SURF_H_LVL_EN_F8,  0xF8  # Horizontal level entrance
.set SURF_H_LVL_EN_F9,  0xF9  # Horizontal level entrance
.set SURF_H_LVL_EN_FA,  0xFA  # Horizontal level entrance
.set SURF_H_LVL_EN_FB,  0xFB  # Horizontal level entrance
.set SURF_H_LVL_EN_FC,  0xFC  # Horizontal level entrance
.set SURF_POOL_WARP,    0xFD  # Pool warp (HMC)
.set SURF_TRAPDOOR,     0xFF  # Bowser 1 trapdoor

# Collision Tris Initiate
.macro colTriInit surfType, triNum
	.byte 0x00
	.byte \surfType
	.hword \triNum
.endm

# Collision Tri
.macro colTri v1, v2, v3
	.hword \v1, \v2, \v3
.endm

# Collision Tri With Special Params
.macro colTriSpecial v1, v2, v3, param
	.hword \v1, \v2, \v3, \param
.endm

# Collision Tris Stop Loading
.macro colTriStop
	.hword 0x0041
.endm

# Special Object Initiate
.macro colSpecialInit num
	.hword 0x0043
	.hword \num
.endm

# Water Boxes Initiate
.macro colWaterBoxInit num
	.hword 0x0044
	.hword \num
.endm


# Water Box
.macro colWaterBox id, x1, z1, x2, z2, y
	.hword \id
	.hword \x1, \z1
	.hword \x2, \z2
	.hword \y
.endm

# End Collision Data
.macro colEnd
	.hword 0x0042
.endm
