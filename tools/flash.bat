@echo off

echo ========================================
echo = Umaii DX Firmware Upgrade Tool v1.0  =
echo = Usage flash.bat ^<firmware.fw^>        =
echo ========================================
echo:

if "%~1"=="" (
	echo [ !! ] Please drag on a firmware file for downloading.
	goto exit
)

echo [ ** ] Use '%~1' as target firmware.

echo [ ** ] Extracting firmware files.
if exist tmp rmdir /s /q tmp
7z e %~1 -o.\tmp\ > nul

echo [ ** ] Flashing .text @ 0x80000000
echo:
echo ========================================
dfu-util --alt 0 --intf 0 --dfuse-address 0x08000000:leave --download "tmp/.text" -w
echo ========================================
echo:

echo [ ** ] Flashing .rucfs @ 0x08010000
echo:
echo ========================================
dfu-util --alt 0 --intf 0 --dfuse-address 0x08010000:leave --download "tmp/.rucfs" -w
echo ========================================
echo:

echo [ OK ] Pass.
rmdir /s /q tmp
echo:

:exit
