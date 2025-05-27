@echo off
setlocal

REM Define o caminho da fonte e o nome do arquivo da fonte
set "FONT_PATH=%~1"
set "FONT_NAME=%~2"

REM Copia a fonte para o diretório de fontes do Windows
copy "%FONT_PATH%" "%WINDIR%\Fonts\%FONT_NAME%"

REM Notifica o sistema sobre a mudança de fonte
RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters

endlocal
