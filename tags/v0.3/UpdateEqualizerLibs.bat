REM IMPORTANT NOTE: The main equalizer dir and the build dir must be in THE SAME FOLDER for this to work
rem i.e. C:\work\equalizer and C:\work\equalizer-build

SET EQPATH=D:\Workspace\3rdparty\equalizer
SET EQPATH-VS9=D:\Workspace\3rdparty\equalizer-win-x86-vs9
SET EQPATH-VS10=D:\Workspace\3rdparty\equalizer-win-x86-vs10

copy %EQPATH-VS9%\libs\client\Debug\Equalizer.lib  .\lib\win-x86-vs9-debug
copy %EQPATH-VS9%\libs\client\Release\Equalizer.lib  .\lib\win-x86-vs9-release

copy %EQPATH-VS9%\bin\Release\Equalizer.dll  .\bin\win-x86-vs9-release
copy %EQPATH-VS9%\bin\Debug\Equalizer.dll  .\bin\win-x86-vs9-debug
copy %EQPATH-VS9%\bin\Release\EqualizerServer.dll  .\bin\win-x86-vs9-release
copy %EQPATH-VS9%\bin\Debug\EqualizerServer.dll  .\bin\win-x86-vs9-debug
copy %EQPATH-VS9%\bin\Release\pthread.dll  .\bin\win-x86-vs9-release
copy %EQPATH-VS9%\bin\Debug\pthread.dll  .\bin\win-x86-vs9-debug

copy %EQPATH-VS10%\libs\client\Debug\Equalizer.lib  .\lib\win-x86-vs10-debug
copy %EQPATH-VS10%\libs\client\Release\Equalizer.lib  .\lib\win-x86-vs10-release

copy %EQPATH-VS10%\bin\Release\Equalizer.dll  .\bin\win-x86-vs10-release
copy %EQPATH-VS10%\bin\Debug\Equalizer.dll  .\bin\win-x86-vs10-debug
copy %EQPATH-VS10%\bin\Release\EqualizerServer.dll  .\bin\win-x86-vs10-release
copy %EQPATH-VS10%\bin\Debug\EqualizerServer.dll  .\bin\win-x86-vs10-debug
copy %EQPATH-VS10%\bin\Release\pthread.dll  .\bin\win-x86-vs10-release
copy %EQPATH-VS10%\bin\Debug\pthread.dll  .\bin\win-x86-vs10-debug

xcopy %EQPATH-VS9%\include .\include\equalizer\ /s /y
xcopy %EQPATH%\libs\*.h .\equalizer\libs\ /s /y
xcopy %EQPATH%\libs\*.ipp .\equalizer\libs\ /s /y



pause