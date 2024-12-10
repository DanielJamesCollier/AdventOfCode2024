set CFLAGS=/MDd /Zi /Od /Wall /GS /guard:cf /sdl /RTC1 -Wno-unsafe-buffer-usage -Wno-declaration-after-statement /std:c17

clang-cl %CFLAGS% day_one/part_one/main.c -o out/day_1_part_1.exe
clang-cl %CFLAGS% day_one/part_two/main.c -o out/day_1_part_2.exe
clang-cl %CFLAGS% day_two/part_one/main.c -o out/day_2_part_1.exe
clang-cl %CFLAGS% day_two/part_two/main.c -o out/day_2_part_2.exe
clang-cl %CFLAGS% day_three/part_one/main.c -o out/day_3_part_1.exe
clang-cl %CFLAGS% day_three/part_two/main.c -o out/day_3_part_2.exe
clang-cl %CFLAGS% day_four/part_one/main.c -o out/day_4_part_1.exe