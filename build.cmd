@ECHO OFF
REM                /Debug   - Create a Debug configuration build environment
REM                /Release - Create a Release configuration build environment
REM                /x86     - Create 32-bit x86 applications
REM                /x64     - Create 64-bit x64 applications
REM                /ia64    - Create 64-bit ia64 applications
REM                /vista   - Windows Vista applications
REM                /xp      - Create Windows XP SP2 applications
REM                /2003    - Create Windows Server 2003 applications
REM                /2008    - Create Windows Server 2008 or Vista SP1 applications
REM                /win7    - Create Windows 7 applications



REM ## This is needed for setenv.cmd later
setlocal EnableDelayedExpansion
REM ## STORE OLD DIR
set olddir=%cd%

REM ## STARTUP COMPILE ENVIRONMENT
SET SDKDIR=c:\Program Files\Microsoft SDKs\Windows\v7.1
REM call "%SDKDIR%\bin\setenv.cmd" %%1 %%2 %%3
call "%SDKDIR%\bin\setenv.cmd" %*

SET NMAKE_PARAM=
IF "%Configuration%"=="Release" (
   SET NMAKE_PARAM=nodebug=1
)

REM ## RESET DIRECTORY
chdir /d %olddir%

REM msbuild.exe /?

nmake clean %NMAKE_PARAM%
nmake %NMAKE_PARAM%

