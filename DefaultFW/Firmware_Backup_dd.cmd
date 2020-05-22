@echo off
SET COM_PORT=COM21
SET FW_IMAGE=%1

esptool.exe --chip esp32 --port %COM_PORT% --baud 921600 read_flash 0 0x400000 %FW_IMAGE%
