@echo off
echo Formatting all .c and .h files in the current directory...
for %%f in (*.c *.h) do clang-format -i "%%f"
echo Done!
