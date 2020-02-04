del /q /f %~dp0*.o
del /q /f %~dp0*.exe
del /q /f %~dp0makefile
move "%~dp0*.cpp" "%~dp0Solutions"
exit