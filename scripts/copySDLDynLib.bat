@echo on
set out=%1
set sdl=%2

(robocopy /S /E %sdl% %out% *.dll		) ^& IF %ERRORLEVEL% GTR 7 exit 1
