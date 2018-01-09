@rem Convenience script to build and run our demo.

@echo OFF
@setlocal EnableDelayedExpansion

call %~dp0\_build\windows.bat

if %ERRORLEVEL% equ 0 (
  pushd %~dp0
  start _build\demo.exe
  popd
) else (
  echo Build failed.
  exit /B 1
)
