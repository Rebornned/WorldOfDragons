@echo off
setlocal

REM Define o nome do arquivo da fonte
set "FONT_NAME=%~1"

REM Remove a fonte do diretório de fontes do Windows
del "%WINDIR%\Fonts\%FONT_NAME%"

REM Notifica o sistema sobre a mudança de fonte
RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters

endlocal
