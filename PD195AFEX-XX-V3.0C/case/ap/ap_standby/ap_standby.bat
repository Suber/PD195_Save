@CD .\Obj
copy ap_standby.hex ..\ap_standby.hex
copy ap_standby.map ..\ap_standby.map
copy ap_standby     ..\ap_standby
@CD ..\
..\..\..\psp_rel\tools\apbuilder.exe -c ap_standby.apc
..\..\..\psp_rel\tools\segcheck ap_standby.map ap_standby.lin ap_standby.apc
copy standby.ap ..\..\fwpkg\