call build.cmd /release /x64 /vista
call build.cmd /release /x86 /xp

cp .\Vista_X64_RETAIL\WinContextMenu.exe ..\bin\WinContextMenu_x64.exe
cp .\XP32_RETAIL\WinContextMenu.exe ..\bin\WinContextMenu_x86.exe

pause