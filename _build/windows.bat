@rem Script to build for Windows using Visual Studio.

@echo OFF
@setlocal EnableDelayedExpansion

@rem Detect if the build has been initiated by Visual Studio.
if defined VisualStudioVersion (
  set IDE=1
) else (
  set IDE=0
)

if not defined TOOLCHAIN (
  if not defined VisualStudioVersion (
    echo Using latest Visual Studio install... 1>&2
    set TOOLCHAIN=latest
  ) else (
    @rem Use the appropriate version of Visual C/C++.
    set TOOLCHAIN=%VisualStudioVersion%
  )
)

@rem Default to x86_64.
if not defined ARCHITECTURE (set ARCHITECTURE=x86_64)

@rem Default to debug builds.
if not defined DEBUG (set DEBUG=1)

set ROOT=%~dp0\..

call %ROOT%\_build\scripts\vc.bat %TOOLCHAIN% windows %ARCHITECTURE%

if not %ERRORLEVEL% equ 0 (
  echo Could not setup environment to target %ARCHITECTURE%!
  exit /B 1
)

if %ARCHITECTURE% equ x86 (
  set CFLAGS=!CFLAGS! /arch:IA32 /fp:except /favor:blend
  set LDFLAGS=!LDFLAGS! /machine:X86
)

if %ARCHITECTURE% equ x86_64 (
  set CFLAGS=!CFLAGS! /fp:except /favor:blend
  set LDFLAGS=!LDFLAGS! /machine:X64
)

@rem TODO: Freestanding!

if %DEBUG% equ 1 (
  set CFLAGS=!CFLAGS! /MTd
  set CFLAGS=!CFLAGS! /Od /Oi
  set CFLAGS=!CFLAGS! /Zi /Fd%ROOT%\_build\demo.pdb
  set CFLAGS=!CFLAGS! /Gm- /GR- /EHa- /GS
  set LDFLAGS=!LDFLAGS! /DEBUG
) else (
  set CFLAGS=!CFLAGS! /MT
  set CFLAGS=!CFLAGS! /O1
  set CFLAGS=!CFLAGS! /Gm- /GR- /EHa- /GS- /Gs134217728
)

cl.exe /nologo /c /W4 %CFLAGS% ^
       /I%ROOT% /I%ROOT%\demo ^
       /Fo%ROOT%\_build\demo.obj ^
       /Tp%ROOT%\demo\build.c

if not %ERRORLEVEL% equ 0 (
  echo Compilation failed.
  exit /B 1
)

link.exe /nologo /subsystem:console %LDFLAGS% ^
         /out:%ROOT%\_build\demo.exe ^
         %ROOT%\_build\demo.obj ^
         kernel32.lib user32.lib gdi32.lib

if not %ERRORLEVEL% equ 0 (
  echo Linking failed.
  exit /B 1
)

echo Built `demo.exe`.
