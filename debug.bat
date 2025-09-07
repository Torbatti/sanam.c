@echo off

:: ----------------------------------------------------------------------------
:: executable name
:: NOTE: Keep debug_exe_name inside debug.bat & build.bat in sync !
set debug_exe_name=sanam_debug

:: ----------------------------------------------------------------------------
:: General Paths
set tooling_path= C:\dev\tooling
set llvm_path=%tooling_path%\llvm-mingw
set raddbg_path=%tooling_path%\raddbg

:: Debuggers path
set raddbg_exe=%RADDBG_PATH%\raddbg.exe
set lldb_exe=%LLVM_PATH%\bin\lldb.exe

:: ----------------------------------------------------------------------------
:: available debuggers | raddbg is default | valgrind is not supported on windows
set raddbg= 1
set lldb= 0 

:: ----------------------------------------------------------------------------
:: set debugger
if %raddbg% == 1 (
    set debugger=%raddbg_exe%
    goto start_debugger
) 
if %lldb% == 1 (
    set debugger=%lldb_exe% 
    goto start_debugger
)
goto no_debugger

:: start debugger
:start_debugger
call %debugger% .\build\%debug_exe_name%.exe

goto end

:no_debugger

:: ----------------------------------------------------------------------------
:: end
:end