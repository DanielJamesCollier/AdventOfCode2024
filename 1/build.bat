set DEBUG_FLAGS=/permissive- /GS /W3 /Zc:wchar_t /ZI /Gm- /Od /sdl /Zc:inline /fp:precise  /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /MDd /FC /EHsc /nologo /diagnostics:column /std:c17 /Fd../out

cl /Fe../out/day_1_part_1.exe %DEBUG_FLAGS% one.c ../djc_arena/djc_arena.c
cl /Fe../out/day_1_part_2.exe %DEBUG_FLAGS% two.c 
