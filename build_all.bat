@echo off
REM Batch file to find and run build.bat in all subdirectories

echo Searching for build.bat files...

REM Loop through all subdirectories and find build.bat
for /r %%d in (.) do (
    if exist "%%d\build.bat" (
        echo Running build.bat in: %%d
        pushd "%%d"
        call build.bat
        popd
    )
)

echo Done executing all build.bat files.
pause
