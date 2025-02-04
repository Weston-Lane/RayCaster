@echo off
set ESC=
set INCLUDE_PATH=../include
set LIBRARY_PATH=../lib
set SRC_PATH=../src/*cpp
set ASSEM_MODULES=../lib/depends/*c
set LIBRARIES=glfw3.lib winmm.lib kernel32.lib opengl32.lib gdi32.lib user32.lib msvcrt.lib shell32.lib
mkdir "build"
rem temporarily moves into this directory to compile the exe
pushd "build"
rem /Zi creates the pdb file for debuging in VS, use command devenv [desired app.exe]; user.lib is the windows UI library

cl /EHcs /Zi /Fe:Finger_Gun /I%INCLUDE_PATH% %SRC_PATH% %ASSEM_MODULES% /link /LIBPATH:%LIBRARY_PATH% %LIBRARIES%
IF "%1"=="-c" (   
echo %ESC%[92mCompiled Sucessfully...%ESC%[0m
) ELSE (
    Finger_Gun.exe
)
popd


 