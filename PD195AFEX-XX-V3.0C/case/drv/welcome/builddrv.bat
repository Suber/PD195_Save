@echo off
if exist ..\fwsp set PRJPATH=..&& goto foundpath
if exist ..\..\fwsp set PRJPATH=..\..&& goto foundpath
if exist ..\..\..\fwsp set PRJPATH=..\..\..&& goto foundpath
if exist ..\..\..\..\fwsp set PRJPATH=..\..\..\..&& goto foundpath
if exist ..\..\..\..\..\fwsp set PRJPATH=..\..\..\..\..&& goto foundpath
if exist ..\..\..\..\..\..\fwsp set PRJPATH=..\..\..\..\..\..&& goto foundpath
:foundpath
for /d %%i in (%PRJPATH%\*) do if exist %%i\psp_rel set SDKPATH=%%i
@echo on
"..\..\..\psp_rel\tools\HexToBin251.EXE" ".\obj\welcome.hex" "..\..\fwpkg\welcome.bin" ff9800 0800
IF {%1}=={} goto nocopy
if exist "..\..\fwpkg\%1" (copy "..\..\fwpkg\welcome.bin" "..\..\fwpkg\%1\welcome.bin")
:nocopy
echo build end 
