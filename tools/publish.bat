@echo off

:: %1 work dir
:: %2 is the path to the .bin file
:: %3 is the path to the rucfs bin file
:: %4 is the output file

set outdir=%1
set workdir=%outdir%\tmp
set program_bin=%2
set rucfs_bin=%3

:: make work dir
mkdir %workdir%

:: prepare the firmware
copy %program_bin% %workdir%\.text > nul
copy %rucfs_bin% %workdir%\.rucfs > nul

:: packup
7z a -tzip -y -aoa -sdel %outdir%\%4 %workdir%\* > nul

:: clean
rmdir /s /q %workdir%

:: print file path
echo firmware path: %outdir%\%4