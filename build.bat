@echo off
set CONF=1_Debug
if "%1" neq "" set CONF=%1
cmake --build build --config %CONF%
