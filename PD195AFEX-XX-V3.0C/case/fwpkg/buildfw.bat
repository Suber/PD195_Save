@echo off
:::::::::::::::::::::::::::::::::::::::::::::::::::::
:: ����1: afi.bin����
:: ����2: buildafi.bat����
:: ����3��cfg�ļ�
:: ����4: fw����
:::::::::::::::::::::::::::::::::::::::::::::::::::::

:::::::::::::::::::::::::::::::::::::::::::::::::::::
:: ��������
:::::::::::::::::::::::::::::::::::::::::::::::::::::
set workDir=%cd%
set uv2Dir=..\..\..\fwsp\uv2
set toolDir=..\..\psp_rel\tools
set afiDir=..\..\psp_rel\bin
set binOriginal=..\..\psp_rel\bin_original

:::::::::::::::::::::::::::::::::::::::::::::::::::::
:: ��������
:::::::::::::::::::::::::::::::::::::::::::::::::::::
IF {%1}=={} (set binAfi=afi.bin) ELSE (set binAfi=%1)
IF {%2}=={} (set batBulidAfi=buildAFI.bat) ELSE (set batBulidAfi=%2)
IF {%3}=={} (set cfgFwimage=fwimage.cfg) ELSE (set cfgFwimage=%3)
IF {%4}=={} (set fwdb=as211anand.fw) ELSE (set fwdb=%4)

:::::::::::::::::::::::::::::::::::::::::::::::::::::
:: ���
:::::::::::::::::::::::::::::::::::::::::::::::::::::
IF exist %binOriginal%\%batBulidAfi% (
    cd %binOriginal%
    call %binOriginal%\%batBulidAfi%
    cd %workDir%
)
%toolDir%\Maker.exe -db %afiDir%\%binAfi% -c %cfgFwimage% -o %fwdb% 





 

