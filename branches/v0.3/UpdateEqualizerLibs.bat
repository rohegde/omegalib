SET EQPATH=D:\Workspace\3rdparty\equalizer
REM SET EQPATH=D:\Workspace\3rdparty\equalizer-0.9.1


copy %EQPATH%\build\VS2008\Win32\Debug\*.lib  .\lib\win-x86-vs9-debug
copy %EQPATH%\build\VS2008\Win32\Release\*.lib  .\lib\win-x86-vs9-release
copy %EQPATH%\build\VS2008\x64\Debug\*.lib  .\lib\win-x64-vs9-debug
copy %EQPATH%\build\VS2008\x64\Release\*.lib  .\lib\win-x64-vs9-release

copy %EQPATH%\build\VS2008\Win32\Debug\*.dll  .\bin\win-x86-vs9-debug
copy %EQPATH%\build\VS2008\Win32\Release\*.dll  .\bin\win-x86-vs9-release
copy %EQPATH%\build\VS2008\x64\Debug\*.dll  .\bin\win-x64-vs9-debug
copy %EQPATH%\build\VS2008\x64\Release\*.dll  .\bin\win-x64-vs9-release

copy %EQPATH%\build\VS2010\Win32\Debug\*.lib  .\lib\win-x86-vs10-debug
copy %EQPATH%\build\VS2010\Win32\Release\*.lib  .\lib\win-x86-vs10-release

copy %EQPATH%\build\VS2010\Win32\Debug\*.dll  .\bin\win-x86-vs10-debug
copy %EQPATH%\build\VS2010\Win32\Release\*.dll  .\bin\win-x86-vs10-release

xcopy %EQPATH%\build\VS2010\Win32\Release\Headers\eq .\include\eq /s

pause