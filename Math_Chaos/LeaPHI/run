#!/bin/bash
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

case "$@" in
    "cmake") echo "+>> running cmake..."
        cmake -Bbuild/ -H.
        cp build/compile_commands.json .
        ;;
    "make"*) echo "+>> running make $2"
        make -j7 -sC build/ $2
        ;;
    *) echo "+>> running leaphi..."
        ./build/leaphi
esac
