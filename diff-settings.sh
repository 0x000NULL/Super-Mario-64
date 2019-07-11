#!/usr/bin/env bash

BASEROM=baseromus
LANG=us

POSITIONAL=()
while [[ $# -gt 0 ]]; do
case "$1" in
    -j)
        BASEROM=baserom
        LANG=jp
        shift
        ;;
    -u)
        BASEROM=baseromus
        LANG=us
        shift
        ;;
    *)
        POSITIONAL+=("$1")
        shift
        ;;
esac
done
set -- "${POSITIONAL[@]}"

LANG_SHORT=${LANG:0:1}
MAPFILE="build/$LANG/sm64.map"
MYDUMP=sm64.$LANG_SHORT.dump
MYIMG=build/$LANG/sm64.$LANG_SHORT.z64
BASEDUMP=$BASEROM.dump
BASEIMG=$BASEROM.z64
MAKEFLAGS="VERSION=$LANG"
