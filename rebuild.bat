@echo off
call clean.bat
call generate.bat
call build.bat %*
