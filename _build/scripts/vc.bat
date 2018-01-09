@rem Script to find an install of particular version of Visual C/C++ and setup
@rem the environment to use it.

@rem Syntax: vc.bat [version] [sdk] [target]
@rem
@rem Options:
@rem   Versions - Version number assigned by Microsoft or `latest`.
@rem   Targets - The architecture to target. Can be `x86` or `x86_64`.
@rem   SDKs - The platform to target. Can be `windows` or `uwp`.

@echo OFF

@rem Older versions of Visual Studio can be found through the registry.

if %1%==8.0 goto find_via_registery
if %1%==9.0 goto find_via_registery
if %1%==10.0 goto find_via_registery
if %1%==11.0 goto find_via_registery
if %1%==12.0 goto find_via_registery
if %1%==14.0 goto find_via_registery

@rem Since Visual Studio 2017, however, installs can only be reliably found
@rem through COM. Instead of dealing with the many poorly documented steps
@rem required to find Visual C/C++ via COM, we use a tool maintained built by
@rem Microsoft for use.

if %1%==15.0 goto find_via_utility

@rem Passing `latest` should pick up Visual Studio 2010 or later.

if %1%==latest goto find_via_utility

echo The requested version of Visual Studio is not supported.
exit /B 1

:find_via_registery

for /F "usebackq tokens=2*" %%i in (`reg query HKLM\SOFTWARE\Microsoft\VisualStudio\%1%\Setup\VC /v "ProductDir" 2^>nul`) do (
  if not "%%j"=="" (
    set VCINSTALLDIR=%%j
    goto found_via_registery
  )
)

for /F "usebackq tokens=2*" %%i in (`reg query HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\%1%\Setup\VC /v "ProductDir" 2^>nul`) do (
  if not "%%j"=="" (
    set VCINSTALLDIR=%%j
    goto found_via_registery
  )
)

@rem We try to find a full version of Visual Studio. If we can't, then we look
@rem for standalone versions, i.e. Express Editions. This is only required for
@rem Visual Studio 2005 to 2010, so this logic can be removed when we stop
@rem supporting them.

for /F "usebackq tokens=2*" %%i in (`reg query HKLM\SOFTWARE\Microsoft\VCExpress\%1%\Setup\VC /v "ProductDir" 2^>nul`) do (
  if not "%%j"=="" (
    set VCINSTALLDIR=%%j
    goto found_via_registery
  )
)

for /F "usebackq tokens=2*" %%i in (`reg query HKLM\SOFTWARE\Wow6432Node\Microsoft\VCExpress\%1%\Setup\VC /v "ProductDir" 2^>nul`) do (
  if not "%%j"=="" (
    set VCINSTALLDIR=%%j
    goto found_via_registery
  )
)

echo Could not find Visual Studio %1% with C/C++ toolset. Is it installed?
exit /B 1

:found_via_registery

@rem Determine host architecture.
if not "%PROCESSOR_ARCHITEW6432%"=="" (
  if "%PROCESSOR_ARCHITEW6432%"=="x86" set HOST=x86
  if "%PROCESSOR_ARCHITEW6432%"=="AMD64" set HOST=amd64
) else (
  if "%PROCESSOR_ARCHITECTURE%"=="x86" set HOST=x86
  if "%PROCESSOR_ARCHITECTURE%"=="AMD64" set HOST=amd64
)

@rem Sanity check target platform.
if not %2%==windows (
  echo Cannot target `%1%` with Visual Studio %1%.
  exit /B 1
)

@rem Map target architecture to Microsoft's monikers.
if %3%==x86 set TARGET=x86
if %3%==x86_64 set TARGET=amd64

@rem Map host and target architecutre to toolset.
if %HOST%==%TARGET% (
  set TOOLSET=%TARGET%
) else (
  set TOOLSET=%HOST%_%TARGET%
)

call "%VCINSTALLDIR%\vcvarsall.bat" %TOOLSET%

exit /B %ERRORLEVEL%

:find_via_utility

@rem Find a copy of `vswhere`, or fetch from Github if necessary (and allowed).

where /Q vswhere.exe

if "%ERRORLEVEL%"=="0" (
  set VSWhere=vswhere.exe
) else (
  set VSWhere=%~dp0\..\tools\cache\vswhere.exe
  set VSWhereVer=2.2.7

  if not exist %~dp0\..\tools\cache\vswhere.exe (
    if defined DO_NOT_FETCH_TOOLS (
      echo Could not find `vswhere`!
      exit /B 1
    ) else (
      setlocal EnableDelayedExpansion

      echo Fetching vswhere@!VSWhereVer! from Github...

      mkdir %~dp0\..\tools 2>NUL
      mkdir %~dp0\..\tools\cache 2>NUL

      PowerShell.exe -NoProfile -NonInteractive -ExecutionPolicy Unrestricted -File %~dp0\download.ps1 https://github.com/Microsoft/vswhere/releases/download/!VSWhereVer!/vswhere.exe !VSWhere!

      if not "!ERRORLEVEL!"=="0" (
        echo Failed to fetch vswhere@!VSWhereVer! from Github!
        exit /B 1
      )

      endlocal
    )
  )
)

@rem Use `vswhere` to find installation path.

for /f "usebackq tokens=*" %%t in (`%VSWhere% -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -format value`) do (
  set VCINSTALLDIR=%%t
)

if not exist "%VCINSTALLDIR%\Common7\Tools\vsdevcmd.bat" (
  echo Could not find Visual Studio %1% with C/C++ toolset. Is it installed?
  exit /B 1
)

:found_via_utility

@rem Determine host architecture.
if not "%PROCESSOR_ARCHITEW6432%"=="" (
  if "%PROCESSOR_ARCHITEW6432%"=="x86" set HOST=x86
  if "%PROCESSOR_ARCHITEW6432%"=="AMD64" set HOST=amd64
) else (
  if "%PROCESSOR_ARCHITECTURE%"=="x86" set HOST=x86
  if "%PROCESSOR_ARCHITECTURE%"=="AMD64" set HOST=amd64
)

@rem Map target platform to Microsoft's monikers.
if %2%==windows set PLATFORM=Desktop
if %2%==uwp set PLATFORM=UWP

@rem Map target architecture to Microsoft's monikers.
if %3%==x86 set TARGET=x86
if %3%==x86_64 set TARGET=amd64

@rem Prevents clobbering of current working directory.
set "VSCMD_START_DIR=%CD%"

call "%VCINSTALLDIR%\Common7\Tools\vsdevcmd.bat" -no_logo -host_arch=%HOST% -arch=%TARGET% -app_platform=%PLATFORM%

exit /B %ERRORLEVEL%
