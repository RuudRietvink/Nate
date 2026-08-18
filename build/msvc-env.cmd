@echo off
setlocal

set "VSWHERE=%~1"
set "VCVARS_ARCH=%~2"
set "MAKE_EXE=%~3"
set "MAKE_GOALS=%~4"
set "VS_INSTALL="

for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS_INSTALL=%%I"

if not defined VS_INSTALL (
	echo Visual C++ installation not found. Set CXX to another compiler.
	exit /b 1
)

set "PATH=%~dp1;%PATH%"
call "%VS_INSTALL%\VC\Auxiliary\Build\vcvarsall.bat" %VCVARS_ARCH% >NUL
if errorlevel 1 exit /b %errorlevel%

set "MSVC_ENV_READY=1"
"%MAKE_EXE%" --no-print-directory %MAKE_GOALS%
exit /b %errorlevel%
