@echo off 
setlocal EnableDelayedExpansion
set workDir=%cd%
set prjdir1=%cd%\..\libsrc
set prjdir2=%cd%\..\drv
set prjdir3=%cd%\..\ap

:::::::::::::::::::::::::::::::::::::::
:: 获取UV3目录
:::::::::::::::::::::::::::::::::::::::
@echo checking UV3 dir ......
@echo off
for /f "tokens=1,2,3,4" %%i in ('reg query "HKEY_CLASSES_ROOT\Applications\UV3.EXE\shell\open\command\"^|findstr /i "Keil"') do (
set str=%%i
if "!str:~-8,-1!"=="UV3.EXE" set UV3DIR=%%i
set str=%%j
if "!str:~-8,-1!"=="UV3.EXE" set UV3DIR=%%j
set str=%%k
if "!str:~-8,-1!"=="UV3.EXE" set UV3DIR=%%k
set str=%%l
if "!str:~-8,-1!"=="UV3.EXE" set UV3DIR=%%l
)

@echo cleaning OPT ......

:::::::::::::::::::::::::::::::::::::::
:: 清理OPT文件
:::::::::::::::::::::::::::::::::::::::
 
cd %prjdir1%
for /f "delims=" %%i in ('dir *.OPT /b /s 2^>nul') do if exist %%i del %%i
cd %prjdir2%
for /f "delims=" %%i in ('dir *.OPT /b /s 2^>nul') do if exist %%i del %%i
cd %prjdir3%
for /f "delims=" %%i in ('dir *.OPT /b /s 2^>nul') do if exist %%i del %%i

if exist "%workDir%\buildlist.txt" del %workDir%\buildlist.txt
if exist "%workDir%\buildlist.log" del %workDir%\buildlist.log
@echo Gen buildlist ......
:::::::::::::::::::::::::::::::::::::::
:: 获取编译列表
:::::::::::::::::::::::::::::::::::::::
 
cd %prjdir1% 
set qc="Target ("
for /f "delims=" %%i in ('dir *.Uv2 /b /s') do (
for /f "tokens=1-2 delims= " %%a in (%%i) do (
if "%%a"=="Target" (
    set pp=%%a
    set tt=%%b
    set tt=!tt:~1,-2!
    echo -b %%i -t "!tt!">>%workDir%\buildlist.txt
)
)
)
 
cd %prjdir2% 
set qc="Target ("
for /f "delims=" %%i in ('dir *.Uv2 /b /s') do (
for /f "tokens=1-2 delims= " %%a in (%%i) do (
if "%%a"=="Target" (
    set pp=%%a
    set tt=%%b
    set tt=!tt:~1,-2!
    echo -b %%i -t "!tt!">>%workDir%\buildlist.txt
)
)
)

cd %prjdir3% 
set qc="Target ("
for /f "delims=" %%i in ('dir *.Uv2 /b /s') do (
for /f "tokens=1-2 delims= " %%a in (%%i) do (
if "%%a"=="Target" (
    set pp=%%a
    set tt=%%b
    set tt=!tt:~1,-2!
    echo -b %%i -t "!tt!">>%workDir%\buildlist.txt
)
)
)
:::::::::::::::::::::::::::::::::::::::
:: 依次编译
:::::::::::::::::::::::::::::::::::::::
 
@echo building ......
for /f "tokens=*" %%a in (%workDir%\buildlist.txt) do %UV3DIR% %%a 2>%workDir%\buildlist.log 
cd %workDir% 


:::::::::::::::::::::::::::::::::::::::
:: 检查结果
:::::::::::::::::::::::::::::::::::::::
@echo check result ......
@echo off
findstr "error" %workDir%\buildlist.log >nul
@echo off
if "%errorlevel%"=="0" goto builderr
goto finish
:builderr
msg %username% "build err"
:finish
 

pause
