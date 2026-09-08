@echo off
setlocal

echo Creating .\bin directory...
if not exist ".\bin" mkdir ".\bin"

echo Running RecompModTool...
.\RecompModTool.exe .\mod.toml .\bin
if errorlevel 1 goto error

echo Zipping output file into .\bin...
zip -j ".\bin\dk64_exit_to_isles.zip" ".\bin\dk64_exit_to_isles.nrm"
if errorlevel 1 goto error

echo Complete
goto end

:error
echo.
echo [ERROR] Script failed.
exit /b 1

:end
endlocal