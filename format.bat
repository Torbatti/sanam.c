@echo off
:: ----------------------------------------------------------------------------
:: General Paths
set tooling_path= C:\dev\tooling
set llvm_path=%tooling_path%\llvm-mingw-20250709-ucrt-x86_64
set clang_fmt_exe=%llvm_path%\bin\clang-format.exe

:: ----------------------------------------------------------------------------
:: Run formatter
call %clang_fmt_exe% -style=file:.clang-format -i .\src\base\*.h .\src\*.c

