@echo off
:: ----------------------------------------------------------------------------
:: executable name
:: NOTE: Keep debug_exe_name inside debug.bat & build.bat in sync !
set debug_exe_name=sanam_debug
set release_exe_name=sanam

:: ----------------------------------------------------------------------------
:: available build procs | debug is default
set build_debug= 1
set build_release= 0

:: available formatter procs | clang-fmt is default
set clang_fmt= 1

:: available compilers | zig is default
set zig= 1

:: ----------------------------------------------------------------------------
:: General Paths
set tooling_path= C:\dev\tooling
set zig_path=%tooling_path%\zig-x86_64-windows-0.14.1
set llvm_path=%tooling_path%\llvm-mingw-20250709-ucrt-x86_64

:: Compilers path
set zig_exe=%zig_path%\zig.exe

:: Debuggers path

:: Formatters path
set clang_fmt_exe=%llvm_path%\bin\clang-format.exe

:: ----------------------------------------------------------------------------
:: General Options
set general_options= -std=c99

:: Linker options
set link_options=

:: Debug options -g -Wall -Wextra -pedantic -Wshadow -Werror -fPIC -fPIE
if %build_debug% == 1 set debug_options= -g 
:: if %build_debug% == 1 set debug_options= -g -Wall -Wextra -pedantic -Wshadow -fPIC -fPIE

:: Release options
if %build_release% == 1 set release_options= -O2 -fPIC -fPIE

:: ----------------------------------------------------------------------------
:: Prep Directories
if not exist build mkdir build
if not exist local mkdir local

:: ----------------------------------------------------------------------------
:: Select compiler
@REM Hate using procs but here we go
if %build_debug% == 1 ( goto select_compiler )
if %build_release% == 1 ( goto select_compiler )
goto no_compiler

:select_compiler
if %zig% == 1 set compiler= %zig_exe% cc

:: Build Step
if %build_debug% == 1 (
    call %compiler% .\src\sanam.c -o .\build\%debug_exe_name%.exe %general_options% %debug_options% %link_options%
    goto end
)
if %build_release% == 1 (
    call %compiler% .\src\sanam.c -o .\build\%release_exe_name%.exe %general_options% %release_options% %link_options%
    goto end
)

:no_compiler

:: ----------------------------------------------------------------------------
:: End
:end
