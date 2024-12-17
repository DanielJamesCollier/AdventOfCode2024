set DEBUG_FLAGS=/permissive- /GS /W3 /Zc:wchar_t /ZI /Gm- /Od /sdl /Zc:inline /fp:precise  /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /MDd /FC /EHsc /nologo /diagnostics:column /std:c17 /Fd../out

cl /Fe../out/day_5_part_1.exe %DEBUG_FLAGS% one.c ../djc_arena/djc_arena.c
