@echo off

if not exist "..\out\tests" (
    echo Creating tests folder...
    mkdir "..\out\tests"
)

cl /Fe../out/tests/test_djc_h.exe %DEBUG_FLAGS% test_djc_h.c
