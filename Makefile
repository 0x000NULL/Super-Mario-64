# Makefile to rebuild SM64 split image

### Default target ###

default: all

### Build Options ###
# Version of the game to build and graphics microcode used
VERSION ?= us
GRUCODE ?= f3d_old
# If COMPARE is 1, check the output sha1sum when building 'all'
COMPARE ?= 1
# If NON_MATCHING is 1, define the NON_MATCHING macro when building
NON_MATCHING ?= 0
# If ENDIAN_IND is 1, enable non-matching code changes that try to ensure
# endianness independence
ENDIAN_IND ?= 0

# Release

ifeq ($(VERSION),jp)
  VERSION_CFLAGS := -DVERSION_JP=1
  VERSION_ASFLAGS := --defsym VERSION_JP=1
  GRUCODE_CFLAGS := -DF3D_OLD
  GRUCODE_ASFLAGS := --defsym F3D_OLD=1
  TARGET := sm64.j
else
ifeq ($(VERSION),us)
  VERSION_CFLAGS := -DVERSION_US=1
  VERSION_ASFLAGS := --defsym VERSION_US=1
  GRUCODE_CFLAGS := -DF3D_OLD
  GRUCODE_ASFLAGS := --defsym F3D_OLD=1
  TARGET := sm64.u
else
ifeq ($(VERSION),eu)
  $(warning Building EU is experimental and is prone to breaking. Try at your own risk.)
  VERSION_CFLAGS := -DVERSION_EU=1
  VERSION_ASFLAGS := --defsym VERSION_US=1 --defsym VERSION_EU=1
  GRUCODE_CFLAGS := -DF3D_OLD
  GRUCODE_ASFLAGS := --defsym F3D_OLD=1
  TARGET := sm64.eu
else
  $(error unknown version "$(VERSION)")
endif
endif
endif

# Microcode

ifeq ($(GRUCODE),f3dex) # Fast3DEX
  GRUCODE_CFLAGS := -DF3DEX_GBI=1
  GRUCODE_ASFLAGS := --defsym F3DEX_GBI_SHARED=1 --defsym F3DEX_GBI=1
  TARGET := $(TARGET).f3dex
  COMPARE := 0
else
ifeq ($(GRUCODE), f3dex2) # Fast3DEX2
  GRUCODE_CFLAGS := -DF3DEX_GBI_2=1
  GRUCODE_ASFLAGS := --defsym F3DEX_GBI_SHARED=1 --defsym F3DEX_GBI_2=1
  TARGET := $(TARGET).f3dex2
  COMPARE := 0
else
ifeq ($(GRUCODE),f3d_new) # Fast3D 2.0H (Shindou)
  GRUCODE_CFLAGS := -DF3D_NEW
  GRUCODE_ASFLAGS := --defsym F3D_NEW=1
  TARGET := $(TARGET).f3d_new
  COMPARE := 0
else
ifeq ($(GRUCODE),f3dzex) # Fast3DZEX (2.0J / Animal Forest - Dōbutsu no Mori)
  $(warning Fast3DZEX is experimental. Try at your own risk.)
  GRUCODE_CFLAGS := -DF3DEX_GBI_2=1
  GRUCODE_ASFLAGS := --defsym F3DEX_GBI_SHARED=1 --defsym F3DZEX_GBI=1
  TARGET := $(TARGET).f3dzex
  COMPARE := 0
endif
endif
endif
endif

ifeq ($(NON_MATCHING),1)
  VERSION_CFLAGS := $(VERSION_CFLAGS) -DNON_MATCHING=1
  COMPARE := 0
endif

ifeq ($(ENDIAN_IND),1)
  VERSION_CFLAGS := $(VERSION_CFLAGS) -DENDIAN_IND=1
  COMPARE := 0
endif

################ Target Executable and Sources ###############

# BUILD_DIR is location where all build artifacts are placed
BUILD_DIR_BASE := build
BUILD_DIR := $(BUILD_DIR_BASE)/$(VERSION)

LIBULTRA := $(BUILD_DIR)/libultra.a
ROM := $(BUILD_DIR)/$(TARGET).z64
ELF := $(BUILD_DIR)/$(TARGET).elf
LD_SCRIPT := sm64.ld
MIO0_DIR := $(BUILD_DIR)/mio0
TEXTURE_DIR := textures
ACTOR_DIR := actors

# Directories containing source files
SRC_DIRS := src src/engine src/game src/goddard src/goddard/dynlists src/audio
ASM_DIRS := asm actors lib data levels assets text
BIN_DIRS := bin

ULTRA_SRC_DIRS := lib/src lib/src/math
ULTRA_ASM_DIRS := lib/asm lib/data
ULTRA_BIN_DIRS := lib/bin

LEVEL_DIRS := $(patsubst levels/%,%,$(dir $(wildcard levels/*/header.s)))

MIPSISET := -mips2 -32

ifeq ($(VERSION),eu)
  OPT_FLAGS := -O2
else
  OPT_FLAGS := -g
endif

# File dependencies and variables for specific files
include Makefile.split

# Source code files
C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES := $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
ULTRA_C_FILES := $(foreach dir,$(ULTRA_SRC_DIRS),$(wildcard $(dir)/*.c))
ULTRA_S_FILES := $(foreach dir,$(ULTRA_ASM_DIRS),$(wildcard $(dir)/*.s))
LEVEL_S_FILES := $(addsuffix header.s,$(addprefix bin/,$(LEVEL_DIRS)))
SEG_IN_FILES := $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.s.in))
SEG_S_FILES := $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.s)) \
               $(foreach file,$(SEG_IN_FILES),$(file:.s.in=.s))

# Object files
O_FILES := $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o)) \
           $(foreach file,$(LEVEL_S_FILES),$(BUILD_DIR)/$(file:.s=.o))

ULTRA_O_FILES := $(foreach file,$(ULTRA_S_FILES),$(BUILD_DIR)/$(file:.s=.o)) \
                 $(foreach file,$(ULTRA_C_FILES),$(BUILD_DIR)/$(file:.c=.o))

# Automatic dependency files
DEP_FILES := $(O_FILES:.o=.d) $(ULTRA_O_FILES:.o=.d)

# Files with NON_MATCHING ifdefs
NON_MATCHING_C_FILES != grep -rl NON_MATCHING $(wildcard src/audio/*.c)
NON_MATCHING_O_FILES = $(foreach file,$(NON_MATCHING_C_FILES),$(BUILD_DIR)/$(file:.c=.o))
NON_MATCHING_DEP = $(BUILD_DIR)/src/audio/non_matching_dep

# Segment elf files
SEG_FILES := $(foreach file,$(SEG_S_FILES),$(BUILD_DIR)/$(file:.s=.elf)) $(ACTOR_ELF_FILES) $(LEVEL_ELF_FILES)

##################### Compiler Options #######################
IRIX_ROOT := tools/ido5.3_compiler

ifeq ($(shell type mips-linux-gnu-ld >/dev/null 2>/dev/null; echo $$?), 0)
  CROSS := mips-linux-gnu-
else
  CROSS := mips64-elf-
endif

AS        := $(CROSS)as
CC        := $(QEMU_IRIX) -silent -L $(IRIX_ROOT) $(IRIX_ROOT)/usr/bin/cc
CPP       := cpp -P
LD        := $(CROSS)ld
AR        := $(CROSS)ar
OBJDUMP   := $(CROSS)objdump
OBJCOPY   := $(CROSS)objcopy

# Check code syntax with host compiler
CC_CHECK := gcc -fsyntax-only -fsigned-char -nostdinc -I include -I $(BUILD_DIR)/include -I src -std=gnu90 -Wall -Wextra -Wno-format-security -D_LANGUAGE_C $(VERSION_CFLAGS) $(GRUCODE_CFLAGS)

ASFLAGS := -march=vr4300 -mabi=32 -I include -I $(BUILD_DIR) $(VERSION_ASFLAGS) $(GRUCODE_ASFLAGS)
CFLAGS = -Wab,-r4300_mul -non_shared -G 0 -Xcpluscomm -Xfullwarn $(OPT_FLAGS) -signed -I include -I $(BUILD_DIR)/include -I src -D_LANGUAGE_C $(VERSION_CFLAGS) $(MIPSISET) $(GRUCODE_CFLAGS)
OBJCOPYFLAGS := --pad-to=0x800000 --gap-fill=0xFF
SYMBOL_LINKING_FLAGS := $(addprefix -R ,$(SEG_FILES))
LDFLAGS := -T undefined_syms.txt -T $(BUILD_DIR)/$(LD_SCRIPT) -Map $(BUILD_DIR)/sm64.map --no-check-sections $(SYMBOL_LINKING_FLAGS)

ifeq ($(shell getconf LONG_BIT), 32)
  # Work around memory allocation bug in QEMU
  export QEMU_GUEST_BASE := 1
else
  # Ensure that gcc treats the code as 32-bit
  CC_CHECK += -m32
endif

####################### Other Tools #########################

# N64 tools
TOOLS_DIR = tools
MIO0TOOL = $(TOOLS_DIR)/mio0
N64CKSUM = $(TOOLS_DIR)/n64cksum
N64GRAPHICS = $(TOOLS_DIR)/n64graphics
N64GRAPHICS_CI = $(TOOLS_DIR)/n64graphics_ci
TEXTCONV = $(TOOLS_DIR)/textconv
IPLFONTUTIL = $(TOOLS_DIR)/iplfontutil
EMULATOR = mupen64plus
EMU_FLAGS = --noosd
LOADER = loader64
LOADER_FLAGS = -vwf
SHA1SUM = sha1sum

# Make tools if out of date
DUMMY != make -s -C tools >&2

###################### Dependency Check #####################

BINUTILS_VER_MAJOR := $(shell $(LD) --version | grep ^GNU | sed 's/^.* //; s/\..*//g')
BINUTILS_VER_MINOR := $(shell $(LD) --version | grep ^GNU | sed 's/^[^.]*\.//; s/\..*//g')
BINUTILS_DEPEND := $(shell expr $(BINUTILS_VER_MAJOR) \>= 2 \& $(BINUTILS_VER_MINOR) \>= 27)
ifeq ($(BINUTILS_DEPEND),0)
$(error binutils version 2.27 required, version $(BINUTILS_VER_MAJOR).$(BINUTILS_VER_MINOR) detected)
endif

ifndef QEMU_IRIX
$(error env variable QEMU_IRIX should point to the qemu-mips binary)
endif

######################## Targets #############################

all: $(ROM)
ifeq ($(COMPARE),1)
	@$(SHA1SUM) -c $(TARGET).sha1
endif

clean:
	$(RM) -r $(BUILD_DIR_BASE)

test: $(ROM)
	$(EMULATOR) $(EMU_FLAGS) $<

load: $(ROM)
	$(LOADER) $(LOADER_FLAGS) $<

libultra: $(BUILD_DIR)/libultra.a

asm/boot.s: $(BUILD_DIR)/lib/bin/ipl3_font.bin

$(BUILD_DIR)/lib/bin/ipl3_font.bin: lib/ipl3_font.png | $(BUILD_DIR)
	$(IPLFONTUTIL) e $< $@

$(BUILD_DIR)/include/text_strings.h: include/text_strings.h.in | $(BUILD_DIR)
	$(TEXTCONV) charmap.txt $< $@

$(BUILD_DIR)/text/%.s: text/$(VERSION)/%.s.in | $(BUILD_DIR)
	$(TEXTCONV) charmap.txt $< $@

build/bin/segment2.o: bin/segment2.s

bin/segment2.s: $(BUILD_DIR)/text/debug.s $(BUILD_DIR)/text/dialog.s $(BUILD_DIR)/text/level.s $(BUILD_DIR)/text/star.s
	touch bin/segment2.s

$(MIO0_DIR)/%.mio0: bin/%.bin
	$(MIO0TOOL) $< $@

ALL_DIRS := $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS) $(ASM_DIRS) $(ULTRA_SRC_DIRS) $(ULTRA_ASM_DIRS) $(ULTRA_BIN_DIRS) $(BIN_DIRS) $(TEXTURE_DIRS) $(addprefix levels/,$(LEVEL_DIRS)) $(addprefix bin/,$(LEVEL_DIRS)) include) $(MIO0_DIR) $(addprefix $(MIO0_DIR)/,$(LEVEL_DIRS))

# Make sure build directory exists before compiling anything
DUMMY != mkdir -p $(ALL_DIRS)

$(BUILD_DIR)/src/game/star_select.o: $(BUILD_DIR)/include/text_strings.h
$(BUILD_DIR)/src/game/file_select.o: $(BUILD_DIR)/include/text_strings.h
$(BUILD_DIR)/src/game/ingame_menu.o: $(BUILD_DIR)/include/text_strings.h

# texture generation
$(BUILD_DIR)/bin/%.rgba16: textures/%.rgba16.png
	$(N64GRAPHICS) -i $@ -g $< -f rgba16

$(BUILD_DIR)/bin/%.ia16: textures/%.ia16.png
	$(N64GRAPHICS) -i $@ -g $< -f ia16

$(BUILD_DIR)/bin/%.ia8: textures/%.ia8.png
	$(N64GRAPHICS) -i $@ -g $< -f ia8

$(BUILD_DIR)/bin/%.ia4: textures/%.ia4.png
	$(N64GRAPHICS) -i $@ -g $< -f ia4

$(BUILD_DIR)/bin/%.ia1: textures/%.ia1.png
	$(N64GRAPHICS) -i $@ -g $< -f ia1

# Color index textures (not used by SM64)
$(BUILD_DIR)/bin/%.ci8: textures/%.ci8.png
	$(N64GRAPHICS_CI) -i $@ -g $< -f ci8

$(BUILD_DIR)/bin/%.ci4: textures/%.ci4.png
	$(N64GRAPHICS_CI) -i $@ -g $< -f ci4

# texture generation 2nd method: rgba16s are preferred (and used
# more often) over the ones listed below due to more colors.
$(BUILD_DIR)/actors/%.rgba16: actors/%.rgba16.png
	$(N64GRAPHICS) -i $@ -g $< -f rgba16

$(BUILD_DIR)/actors/%.ia16: actors/%.ia16.png
	$(N64GRAPHICS) -i $@ -g $< -f ia16

$(BUILD_DIR)/actors/%.ia8: actors/%.ia8.png
	$(N64GRAPHICS) -i $@ -g $< -f ia8

$(BUILD_DIR)/actors/%.ia4: actors/%.ia4.png
	$(N64GRAPHICS) -i $@ -g $< -f ia4

$(BUILD_DIR)/actors/%.ia1: actors/%.ia1.png
	$(N64GRAPHICS) -i $@ -g $< -f ia1

# Color index textures (not used by SM64)
$(BUILD_DIR)/actors/%.ci8: actors/%.ci8.png
	$(N64GRAPHICS_CI) -i $@ -g $< -f ci8

$(BUILD_DIR)/actors/%.ci4: actors/%.ci4.png
	$(N64GRAPHICS_CI) -i $@ -g $< -f ci4

# texture generation 3rd method: rgba16s are preferred (and used
# more often) over the ones listed below due to more colors.
$(BUILD_DIR)/levels/%.rgba16: levels/%.rgba16.png
	$(N64GRAPHICS) -i $@ -g $< -f rgba16

$(BUILD_DIR)/levels/%.ia16: levels/%.ia16.png
	$(N64GRAPHICS) -i $@ -g $< -f ia16

$(BUILD_DIR)/levels/%.ia8: levels/%.ia8.png
	$(N64GRAPHICS) -i $@ -g $< -f ia8

$(BUILD_DIR)/levels/%.ia4: levels/%.ia4.png
	$(N64GRAPHICS) -i $@ -g $< -f ia4

$(BUILD_DIR)/levels/%.ia1: levels/%.ia1.png
	$(N64GRAPHICS) -i $@ -g $< -f ia1

# Color index textures (not used by SM64)
$(BUILD_DIR)/levels/%.ci8: levels/%.ci8.png
	$(N64GRAPHICS_CI) -i $@ -g $< -f ci8

$(BUILD_DIR)/levels/%.ci4: levels/%.ci4.png
	$(N64GRAPHICS_CI) -i $@ -g $< -f ci4

# compressed segment generation
$(BUILD_DIR)/bin/%.o: bin/%.s
	$(AS) $(ASFLAGS) --no-pad-sections -o $@ $<

# compressed segment generation (actors)
$(BUILD_DIR)/bin/%.o: actors/%.s
	$(AS) $(ASFLAGS) --no-pad-sections -o $@ $<

$(BUILD_DIR)/bin/%/leveldata.o: levels/%/leveldata.s
	$(AS) $(ASFLAGS) --no-pad-sections -o $@ $<

$(BUILD_DIR)/bin/%/header.o: levels/%/header.s $(MIO0_DIR)/%/leveldata.mio0 levels/%/script.s
	$(AS) $(ASFLAGS) --no-pad-sections -o $@ $<

# TODO: ideally this would be `-Trodata-segment=0x07000000` but that doesn't set the address
$(BUILD_DIR)/bin/%.elf: $(BUILD_DIR)/bin/%.o
	$(LD) -e 0 -Ttext=$(SEGMENT_ADDRESS) -Map $@.map -o $@ $<

# Override for level.elf, which otherwise matches the above pattern
.SECONDEXPANSION:
$(BUILD_DIR)/bin/%/leveldata.elf: $(BUILD_DIR)/bin/%/leveldata.o $(BUILD_DIR)/bin/$$(TEXTURE_BIN).elf
	$(LD) -e 0 -Ttext=$(SEGMENT_ADDRESS) -Map $@.map --just-symbols=$(BUILD_DIR)/bin/$(TEXTURE_BIN).elf -o $@ $<

$(BUILD_DIR)/bin/%.bin: $(BUILD_DIR)/bin/%.elf
	$(OBJCOPY) -j .rodata $< -O binary $@

$(MIO0_DIR)/%.mio0: $(BUILD_DIR)/bin/%.bin
	$(MIO0TOOL) $< $@

$(MIO0_DIR)/%.mio0.o: $(MIO0_DIR)/%.mio0.s
	$(AS) $(ASFLAGS) -o $@ $<

$(MIO0_DIR)/%.mio0.s: $(MIO0_DIR)/%.mio0
	printf ".section .data\n\n.incbin \"$<\"\n" > $@

# Source code
$(BUILD_DIR)/src/goddard/%.o: OPT_FLAGS := -g
$(BUILD_DIR)/src/goddard/%.o: MIPSISET := -mips1
$(BUILD_DIR)/src/audio/%.o: CC := python3 tools/asm_processor/build.py $(CC) -- $(AS) $(ASFLAGS) --
$(BUILD_DIR)/src/audio/%.o: OPT_FLAGS := -O2 -Wo,-loopunroll,0
$(BUILD_DIR)/src/audio/dma.o: OPT_FLAGS := -O2 -framepointer -Wo,-loopunroll,0
$(BUILD_DIR)/lib/src/%.o: OPT_FLAGS :=
$(BUILD_DIR)/lib/src/math/ll%.o: MIPSISET := -mips3 -32
$(BUILD_DIR)/lib/src/math/%.o: OPT_FLAGS := -O2
$(BUILD_DIR)/lib/src/math/ll%.o: OPT_FLAGS :=
$(BUILD_DIR)/lib/src/ldiv.o: OPT_FLAGS := -O2
$(BUILD_DIR)/lib/src/string.o: OPT_FLAGS := -O2
$(BUILD_DIR)/lib/src/gu%.o: OPT_FLAGS := -O3
$(BUILD_DIR)/lib/src/al%.o: OPT_FLAGS := -O3

# Rebuild files with '#ifdef NON_MATCHING' when that macro changes.
$(NON_MATCHING_O_FILES): $(NON_MATCHING_DEP).$(NON_MATCHING)
$(NON_MATCHING_DEP).$(NON_MATCHING):
	@rm -f $(NON_MATCHING_DEP).*
	touch $@

$(BUILD_DIR)/lib/src/math/%.o: lib/src/math/%.c
	@$(CC_CHECK) -MMD -MP -MT $@ -MF $(BUILD_DIR)/lib/src/math/$*.d $<
	$(CC) -c $(CFLAGS) -o $@ $<
	tools/patch_libultra_math $@ || rm $@

$(BUILD_DIR)/%.o: %.c
	@$(CC_CHECK) -MMD -MP -MT $@ -MF $(BUILD_DIR)/$*.d $<
	$(CC) -c $(CFLAGS) -o $@ $<


$(BUILD_DIR)/%.o: %.s $(MIO0_FILES)
	$(AS) $(ASFLAGS) -MD $(BUILD_DIR)/$*.d -o $@ $<

$(BUILD_DIR)/$(LD_SCRIPT): $(LD_SCRIPT)
	$(CPP) $(VERSION_CFLAGS) -I include/ -DBUILD_DIR=$(BUILD_DIR) -o $@ $<

$(BUILD_DIR)/libultra.a: $(ULTRA_O_FILES)
	$(AR) rcs -o $@ $(ULTRA_O_FILES)

$(ELF): $(O_FILES) $(MIO0_OBJ_FILES) $(SEG_FILES) $(BUILD_DIR)/$(LD_SCRIPT) undefined_syms.txt $(BUILD_DIR)/libultra.a
	$(LD) -L $(BUILD_DIR) $(LDFLAGS) -o $@ $(O_FILES)$(LIBS) -lultra

$(ROM): $(ELF)
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $(@:.z64=.bin) -O binary
	$(N64CKSUM) $(@:.z64=.bin) $@

$(BUILD_DIR)/$(TARGET).objdump: $(ELF)
	$(OBJDUMP) -D $< > $@



.PHONY: all clean default diff test load libultra
.PRECIOUS: $(BUILD_DIR)/mio0/%.mio0 $(BUILD_DIR)/bin/%.elf $(BUILD_DIR)/mio0/%.mio0.s

# Remove built-in rules, to improve performance
MAKEFLAGS += --no-builtin-rules

-include $(DEP_FILES)

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
