@echo off
setlocal enabledelayedexpansion

:: Set the directory where the .exe files are located
set "outFolder=out"

:: Check if the directory exists
if not exist "%outFolder%" (
    echo The directory "%outFolder%" does not exist.
    exit /b
)

:: Loop through each .exe file in the out folder and execute it
for %%f in ("%outFolder%\*.exe") do (
    echo Running %%f...
    "%%f"
)

echo All executables have been run.
endlocal
