
..\..\..\psp_rel\tools\builddriver51 .\UI_Release\obj\drv_ui.hex 
copy .\UI_Release\obj\drv_ui.drv  ..\..\fwpkg\
IF {%1}=={} goto nocopy
if exist "..\..\fwpkg\%1" (copy "..\..\fwpkg\drv_ui.drv" "..\..\fwpkg\%1\drv_ui.drv")
:nocopy
echo build end 
