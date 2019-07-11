#!/usr/bin/env python3
import sys
import os.path

lang = None

for arg in sys.argv:
    if arg == '-j':
        lang = 'jp'
    if arg == '-u':
        lang = 'us'

if lang is None:
    lang = 'us'
    best = 0
    for path in ['build/us/sm64.u.z64', 'build/jp/sm64.j.z64']:
        try:
            if os.path.getmtime(path) > best:
                lang = path.split('/')[1]
        except Exception:
            pass
    print("Assuming language " + lang)

baserom = 'baserom' if lang == 'jp' else 'baseromus'
baseimg = baserom + '.z64'
basemap = 'sm64.' + lang[0] + '.map'

myimg = 'build/' + lang + '/sm64.' + lang[0] + '.z64'
mymap = 'build/' + lang + '/sm64.map'

if os.path.isfile('expected/' + mymap):
    basemap = 'expected/' + mymap

required_files = [baseimg, myimg, mymap]
if any(not os.path.isfile(path) for path in required_files):
    print(', '.join(required_files[:-1]) + " and " + required_files[-1] + " must exist.")
    exit(1)

mybin = open(myimg, 'rb').read()
basebin = open(baseimg, 'rb').read()

if len(mybin) != len(basebin):
    print("Modified ROM has different size...")
    exit(1)

def search_map(rom_addr):
    ram_offset = None
    last_ram = 0
    last_rom = 0
    last_fn = '<start of rom>'
    last_file = '<no file>'
    with open(mymap) as f:
        for line in f:
            if 'load address' in line:
                # Example: ".boot           0x0000000004000000     0x1000 load address 0x0000000000000000"
                ram = int(line[16:16+18], 0)
                rom = int(line[59:59+18], 0)
                ram_offset = ram - rom
                continue

            if ram_offset is None or '=' in line or '*fill*' in line or ' 0x' not in line:
                continue
            ram = int(line[16:16+18], 0)
            rom = ram - ram_offset
            fn = line.split()[-1]
            if '0x' in fn:
                ram_offset = None
                continue
            if rom > rom_addr:
                return 'in {} (ram 0x{:08x}, rom 0x{:x}, {})'.format(last_fn, last_ram, last_rom, last_file)
            last_ram = ram
            last_rom = rom
            last_fn = fn
            if '/' in fn:
                last_file = fn
    return 'at end of rom?'

def parse_map(fname):
    ram_offset = None
    cur_file = '<no file>'
    syms = {}
    prev_sym = None
    with open(fname) as f:
        for line in f:
            if 'load address' in line:
                ram = int(line[16:16+18], 0)
                rom = int(line[59:59+18], 0)
                ram_offset = ram - rom
                continue

            if ram_offset is None or '=' in line or '*fill*' in line or ' 0x' not in line:
                continue
            ram = int(line[16:16+18], 0)
            rom = ram - ram_offset
            fn = line.split()[-1]
            if '0x' in fn:
                ram_offset = None
            elif '/' in fn:
                cur_file = fn
            else:
                syms[fn] = (rom, cur_file, prev_sym)
                prev_sym = fn
    return syms

def map_diff():
    map1 = parse_map(mymap)
    map2 = parse_map(basemap)
    min_ram = None
    found = None
    for sym, addr in map1.items():
        if sym not in map2:
            continue
        if addr[0] != map2[sym][0]:
            if min_ram is None or addr[0] < min_ram:
                min_ram = addr[0]
                found = (sym, addr[1], addr[2])
    if min_ram is None:
        return False
    else:
        print()
        print("Map appears to have shifted just before {} ({}) -- in {}?".format(found[0], found[1], found[2]))
        if found[2] is not None and found[2] not in map2:
            print()
            print("(Base map file {} out of date due to renamed symbols, so result may be imprecise.)".format(basemap))
        return True

def hexbytes(bs):
    return ":".join("{:02x}".format(c) for c in bs)

found_first = False
diffs = 0
shift_cap = 1000
for i in range(24, len(mybin), 4):
    # (mybin[i:i+4] != basebin[i:i+4], but that's slightly slower in CPython...)
    if mybin[i] != basebin[i] or mybin[i+1] != basebin[i+1] or mybin[i+2] != basebin[i+2] or mybin[i+3] != basebin[i+3]:
        if not found_first:
            print("First difference at ROM addr " + hex(i) + ", " + search_map(i))
            print("Bytes:", hexbytes(mybin[i:i+4]), 'vs', hexbytes(basebin[i:i+4]))
            found_first = True
        diffs += 1
        if diffs > shift_cap:
            break
if not found_first:
    print("No differences!")
    exit()
definite_shift = (diffs > shift_cap)
if not definite_shift:
    print(str(diffs) + " differing word(s).")

if diffs > 100:
    if not os.path.isfile(basemap):
        if definite_shift:
            print("Tons of differences, must be a shifted ROM.")
        print("To find ROM shifts, copy a clean .map file to " + basemap + " and rerun this script.")
        exit()

    if not map_diff():
        print("No ROM shift{}.".format(" (!?)" if definite_shift else ""))
